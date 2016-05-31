#include "Photobooth.h"
#include "StatCollector.h"
#include "server/Server.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci;
using namespace ci::app;

Photobooth::Photobooth(config::ISettingsRef config)
{
	logger().log("~~~ Photobooth.Created ~~~");

	init(config);
	setType(ScreenId::PHOTOBOOTH);
	create();
}

Photobooth::~Photobooth()
{	
	logger().log("~~~ Photobooth.Destruct ~~~");

	updateSignal.disconnect();
	removeListeners();
	locations.clear();
}

void Photobooth::init(config::ISettingsRef config)
{
	settings		 = static_pointer_cast<config::PhotoboothSettings>(config);
	statSettings     = static_pointer_cast<StatCollector>(settings);
}

void Photobooth::create()
{	
	photoStorage	 = PhotoStorageRef(new PhotoStorage());
	dbRecord		 = shared_ptr<DataBaseRecord>(new DataBaseRecord());
	
	photoInstruction = PhotoInstructionRef(new PhotoInstruction(settings));
	photoFilter		 = PhotoFilterRef(new PhotoFilter(settings,		  photoStorage));
	photoTimer		 = PhotoTimerRef(new PhotoTimer(settings,		  photoStorage));	
	photoShooting	 = PhotoShootingRef(new PhotoShooting(settings,   photoStorage));
	photoChoosing	 = PhotoChoosingRef(new PhotoChoosing(settings,   photoStorage));	
	photoTemplate	 = PhotoTemplateRef(new PhotoTemplate(settings,   photoStorage));
	photoSharing	 = SocialLocationRef(new SocialLocation(settings, statSettings, photoStorage));


	initLocations();
	cameraSetup();
	chrome().init();
}

void Photobooth::start()
{
	logger().log("~~~ Photobooth.Start ~~~");

	server().gameEnter(settings->getAppID());
	gl::enableAlphaBlending();
	
	updateSignal = App::get()->getSignalUpdate().connect(bind(&Photobooth::update, this));

	index = 0;
	currentLocation = locations[index];
	initShowAnimation();
	delaycall(bind(&Photobooth::goToPhotoInstructionTimeOut, this), settings->GoToScreenSaverTime, "toPhotoBoothScreenSaver");
	cameraCanon().setAutoReconnect(true);
}

void Photobooth::goToPhotoInstructionTimeOut()
{
	logger().log("~~~ Photobooth.Timeot GoTo First Screen ~~~");

	if (!currentLocation->equalLocations<PhotoInstruction>(currentLocation))
	{
		server().gameFail(settings->getAppID());
		saveDbRecord();
		currentLocation->stop();
		gotoFirstlocation();
	}	
}

void Photobooth::showAnimationComplete()
{
	for (auto loc : locations)
	{
		loc->connectEventHandler(&Photobooth::nextLocationHandler,     this, IGameLocation::NEXT_LOC);
		loc->connectEventHandler(&Photobooth::beginAnimHandler, this, IGameLocation::BEGIN_ANIM);
		loc->connectEventHandler(&Photobooth::completeAnimHandler, this, IGameLocation::COMPLETE_ANIM);
		loc->connectEventHandler(&Photobooth::disableGameCloseHandler, this, IGameLocation::DISABLE_GAME_CLOSE);
		loc->connectEventHandler(&Photobooth::enableGameCloseHandler, this, IGameLocation::ENABLE_GAME_CLOSE);
		loc->connectEventHandler(&Photobooth::closeLocationHandler, this, IGameLocation::CLOSE_LOCATION);
		loc->setDbRecord(dbRecord);
	}		

	photoChoosing->connectEventHandler(&Photobooth::reshotHandler, this, PhotoChoosing::RESHOT_LOC);	
	callback(ENABLE_GAME_CLOSE);

	if(cameraCanon().isConnected())
	{
		state = DRAW;
		currentLocation->start();
	}	
}

void Photobooth::beginAnimHandler()
{
	callback(DISABLE_GAME_CLOSE);
}

void Photobooth::completeAnimHandler()
{	
	callback(ENABLE_GAME_CLOSE);
}

void Photobooth::enableGameCloseHandler()
{
	callback(ENABLE_GAME_CLOSE);
}

void Photobooth::closeLocationHandler()
{
	callback(CLOSE_LOCATION);	
}

void Photobooth::disableGameCloseHandler()
{
	callback(DISABLE_GAME_CLOSE); 
}

void Photobooth::stop()
{
	logger().log("~~~ Photobooth.Stop ~~~");

	if (!currentLocation->equalLocations<PhotoInstruction>(currentLocation))
	{
		saveDbRecord();
	}

	updateSignal.disconnect();

	for (auto it : locations)
	{
		it->stop();
	}		

	animX.stop();
	animX1.stop();
	alpha.stop();

	clearDelaycall();
	cameraCanon().setAutoReconnect(false);
}

void Photobooth::reset()
{
	logger().log("~~~ Photobooth.Reset ~~~");

	for (auto it : locations)
	{
		it->reset(settings);
	}		
}

void Photobooth::initLocations()
{
	removeListeners();

	locations.clear();
	locations.push_back(photoInstruction);
	locations.push_back(photoFilter);
	locations.push_back(photoTimer);
	locations.push_back(photoShooting);
	locations.push_back(photoChoosing);
	locations.push_back(photoTemplate);	
	locations.push_back(photoSharing);
}

void Photobooth::cameraSetup()
{
	cameraCanon().setup();
	cameraCanon().startLiveView();
	cameraCanon().setDownloadDirectory(settings->getPhotoDownloadDirectory());
	cameraCanon().setImageQuality();
	cameraCanon().setAutoReconnect(true);
}

void Photobooth::nextLocationHandler()
{
	if (++index >= locations.size())
	{
		saveDbRecord();		
		gotoFirstlocation();
	}		
	else
	{
		if (currentLocation->equalLocations<PhotoTemplate>(locations[index]) && !settings->isPrinterOn())
		{
			++index;
		}		
		
		currentLocation = locations[index];
		currentLocation->start();
		
		clearDelaycall("toPhotoBoothScreenSaver");
		delaycall(bind(&Photobooth::goToPhotoInstructionTimeOut, this), settings->GoToScreenSaverTime, "toPhotoBoothScreenSaver");
	}	
}

void Photobooth::reshotHandler()
{
	gotoFirstlocation();
}

void Photobooth::gotoFirstlocation()
{
	index = 0;
	currentLocation = locations[index];
	currentLocation->start();
}

void Photobooth::update()
{
	handleCameraConnection();

	if (state != CAMERA_DISCONNECT)
	{
		currentLocation->update();
	}		
}

void Photobooth::handleCameraConnection()
{

#ifndef Photobooth_DEBUG
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

void Photobooth::draw()
{	
	switch (state)
	{
		case SHOW_ANIM:
			screenshotDraw();
			gl::pushMatrices();
			gl::translate(animX, 0.0f);
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

void Photobooth::drawCameraErrorPopup()
{
	gl::draw(settings->getTexture("popupErrorBg"));
	auto tex = settings->getTexture("cameraErrorText");
	gl::draw(tex, Vec2f(0.5f * (1080.0f - tex.getWidth()), 766.0f - 0.5f * tex.getHeight()));
}

void Photobooth::removeListeners()
{
	for (auto loc : locations)
	{
		loc->disconnectEventHandler(IGameLocation::NEXT_LOC);
		loc->disconnectEventHandler(IGameLocation::BEGIN_ANIM);
		loc->disconnectEventHandler(IGameLocation::COMPLETE_ANIM);
		loc->disconnectEventHandler(IGameLocation::DISABLE_GAME_CLOSE);
		loc->disconnectEventHandler(IGameLocation::ENABLE_GAME_CLOSE);
		loc->disconnectEventHandler(IGameLocation::CLOSE_LOCATION);		
	}	

	photoChoosing->disconnectEventHandler(PhotoChoosing::RESHOT_LOC);
	cameraCanon().setAutoReconnect(false);
}

void Photobooth::saveDbRecord()
{
	std::string basePath = settings->getDataBasePath();
	std::string baseName = settings->getDataBaseName();

	//collect data
	auto statCollector = static_pointer_cast<StatCollector>(settings);
	statCollector->saveStatData(dbRecord, basePath, baseName);
	statCollector->addPlayedGame();
	dbRecord->clear();	
}

