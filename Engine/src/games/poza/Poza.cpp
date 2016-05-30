#include "Poza.h"
#include "CameraAdapter.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::poza;


float Poza::humanHeight = 0;

Poza::Poza(ISettingsRef config)
{
	logger().log("~~~ Poza.Created ~~~");
	init(config);
	setType(ScreenId::KOTOPOZA);
	create();
}

Poza::~Poza()
{
	logger().log("~~~ Poza.Destruct ~~~");
	updateSignal.disconnect();
	locations.clear();
	removeListeners();
}

void Poza::start()
{
	logger().log("~~~ Poza.Start ~~~");
	index = 0;
	currentLocation = locations[index];

	updateSignal = App::get()->getSignalUpdate().connect(bind(&Poza::update, this));

	kinectSetup();
	cameraCanon().setAutoReconnect(true);
	initShowAnimation();
}

void Poza::init(ISettingsRef config)
{
	settings	    = std::static_pointer_cast<PozaSettings>(config);
	statSettings    = std::static_pointer_cast<StatCollector>(settings);
}

void Poza::create()
{
	pozaData.data   = settings->getPozaData();
	pozaData.shuffleDataBase();

	photoStorage	= PhotoStorageRef(new PhotoStorage());	
	dbRecord		= shared_ptr<DataBaseRecord>(new DataBaseRecord());
	humanModel		= shared_ptr<HumanModel>(new HumanModel());

	pozaInstruction = PozaInstructionRef(new PozaInstruction(settings));
	triMeters		= ThreeMetersRef(new ThreeMeters(settings));
	handsUp			= HandsUpRef(new HandsUp(settings, humanModel));
	timer			= PozaTimer1Ref(new PozaTimer1(settings));
	game			= PozaGameRef(new PozaGame(settings, photoStorage, gamesScore, pozaData, humanModel));
	printer			= PrinterRef(new Printer(settings, photoStorage, gamesScore));
	social			= SocialLocationRef(new SocialLocation(settings, statSettings, photoStorage));

	initLocations();

	chrome().init();
	cameraSetup();	
}

void Poza::cameraSetup()
{
	cameraCanon().setup();
	cameraCanon().startLiveView();
	cameraCanon().setDownloadDirectory(settings->getPhotoDownloadDirectory());
	cameraCanon().setImageQuality();
	cameraCanon().setAutoReconnect(true);
}

void Poza::kinectSetup()
{
	kinect().create();
	kinect().start();	
}

void Poza::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);

	for (auto loc : locations)
	{
		loc->connectEventHandler(&Poza::firstLocationHandler,	this, IGameLocation::FIRST_LOC);
		loc->connectEventHandler(&Poza::nextLocationHandler,	this, IGameLocation::NEXT_LOC);
		loc->connectEventHandler(&Poza::beginAnimHandler,		this, IGameLocation::BEGIN_ANIM);
		loc->connectEventHandler(&Poza::completeAnimHandler,	this, IGameLocation::COMPLETE_ANIM);
		loc->connectEventHandler(&Poza::disableGameCloseHandler,this, IGameLocation::DISABLE_GAME_CLOSE);
		loc->connectEventHandler(&Poza::enableGameCloseHandler, this, IGameLocation::ENABLE_GAME_CLOSE);
		loc->connectEventHandler(&Poza::closeLocationHandler,   this, IGameLocation::CLOSE_LOCATION);
		loc->setDbRecord(dbRecord);
	}

#ifndef Poza_DEBUG
	if (cameraCanon().isConnected() && kinect().deviceExist())
#endif
	{
		state = DRAW;
		currentLocation->start();
	}
}

void Poza::gotoFirstlocation()
{
	index = 0;
	currentLocation = locations[index];
	currentLocation->start();
}

void Poza::firstLocationHandler()
{
	gotoFirstlocation();
}

void Poza::nextLocationHandler()
{	
	if (++index >= locations.size())
	{
		saveDbRecord();
		gotoFirstlocation();
	}
	else 
	{
		currentLocation = locations[index];
		currentLocation->start();

		clearDelaycall("toPhotoBoothScreenSaver");
		delaycall(bind(&Poza::goToPhotoInstructionTimeOut, this), settings->GoToScreenSaverTime, "toPhotoBoothScreenSaver");
	}
}

void Poza::goToPhotoInstructionTimeOut()
{
	logger().log("~~~ Poza.Timeot GoTo First Screen ~~~");

	if (!currentLocation->equalLocations<PozaInstruction>(currentLocation))
	{
		saveDbRecord();
		currentLocation->stop();
		gotoFirstlocation();
	}
}

void Poza::stop()
{
	logger().log("~~~ Poza.Stop ~~~");

	updateSignal.disconnect();

	for (auto it : locations)
	{
		it->stop();
	}

	clearDelaycall();
	cameraCanon().setAutoReconnect(false);
	

	if (!currentLocation->equalLocations<PozaInstruction>(currentLocation))
	{
		saveDbRecord();
	}

	kinect().stop();
}

void Poza::reset()
{

}

void Poza::update()
{
	handleCameraConnection();
#ifndef Poza_DEBUG
	if (state != CAMERA_DISCONNECT)
#endif
	{
		if (currentLocation)
		{
			currentLocation->update();
		}
	}
}

void Poza::initLocations()
{
	removeListeners();
	locations.clear();
	locations.push_back(pozaInstruction);
	locations.push_back(triMeters);
	locations.push_back(handsUp);
	//locations.push_back(timer);
	locations.push_back(game);
	locations.push_back(printer);
	locations.push_back(social);
}

void Poza::closeMouseUpHandler(IButton& button)
{
	closeLocationSignal();
}

void Poza::removeListeners()
{
	for (auto loc : locations)
	{		
		loc->disconnectEventHandler(IGameLocation::FIRST_LOC);
		loc->disconnectEventHandler(IGameLocation::NEXT_LOC);
		loc->disconnectEventHandler(IGameLocation::BEGIN_ANIM);
		loc->disconnectEventHandler(IGameLocation::COMPLETE_ANIM);
		loc->disconnectEventHandler(IGameLocation::DISABLE_GAME_CLOSE);
		loc->disconnectEventHandler(IGameLocation::ENABLE_GAME_CLOSE);
		loc->disconnectEventHandler(IGameLocation::CLOSE_LOCATION);
	}	

	cameraCanon().setAutoReconnect(false);
}

void Poza::draw()
{
	switch (state)
	{
	case SHOW_ANIM:
		//screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		gl::color(Color::black());
		currentLocation->draw();
		gl::popMatrices();
		break;

	case DRAW:
		currentLocation->draw();
		break;

	case CAMERA_DISCONNECT:
		drawCameraErrorPopup();
		break;
	}
}

void Poza::handleCameraConnection()
{

#ifndef Poza_DEBUG
	if (!cameraCanon().isConnected() && state != CAMERA_DISCONNECT)
	{
		state = CAMERA_DISCONNECT;
		currentLocation->stop();
		clearDelaycall("toPhotoBoothScreenSaver");
	}
	else if (cameraCanon().isConnected() && state == CAMERA_DISCONNECT)
	{
		state = DRAW;
		gotoFirstlocation();
	}
#else
	if (state == SHOW_ANIM)
	{
		state = DRAW;
		gotoFirstlocation();
	}
#endif
}

void Poza::drawCameraErrorPopup()
{
	gl::draw(settings->getTexture("popupErrorBg"));
	//auto tex = settings->getTexture("cameraErrorText");
	//gl::draw(tex, Vec2f(0.5f * (1080.0f - tex.getWidth()), 766.0f - 0.5f * tex.getHeight()));
}

void Poza::beginAnimHandler()
{
	callback(DISABLE_GAME_CLOSE);
}

void Poza::completeAnimHandler()
{
	callback(ENABLE_GAME_CLOSE);
}

void Poza::enableGameCloseHandler()
{
	callback(ENABLE_GAME_CLOSE);
}

void Poza::closeLocationHandler()
{
	callback(CLOSE_LOCATION);
}

void Poza::disableGameCloseHandler()
{
	callback(DISABLE_GAME_CLOSE);
}

void Poza::saveDbRecord()
{
	std::string basePath = settings->getDataBasePath();
	std::string baseName = settings->getDataBaseName();

	//collect data
	auto statCollector = static_pointer_cast<StatCollector>(settings);
	statCollector->saveStatData(dbRecord, basePath, baseName);
	statCollector->addPlayedGame();
	dbRecord->clear();
}
