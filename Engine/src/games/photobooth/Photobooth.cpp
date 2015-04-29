#include "Photobooth.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci;
using namespace ci::app;

Photobooth::Photobooth(ISettingsRef config)
{
	console()<<"Photobooth CREATED::: "<<endl;
	init(config);
	setType(ScreenId::PHOTOBOOTH);
	create();
}

Photobooth::~Photobooth()
{	
	console()<<"~~~~~~~~~~~~~~~ Photobooth destructor ~~~~~~~~~~~~~~~"<<endl;
	updateSignal.disconnect();	
	removeListeners();
	locations.clear();
}

void Photobooth::init(ISettingsRef config)
{
	settings = static_pointer_cast<PhotoboothSettings>(config);	
}

void Photobooth::create()
{	
	photoStorage	 = PhotoStorageRef(new PhotoStorage());

	photoInstruction = PhotoInstructionRef(new PhotoInstruction(settings));
	photoFilter		 = PhotoFilterRef(new PhotoFilter(settings, photoStorage));
	photoTimer		 = PhotoTimerRef(new PhotoTimer(settings, photoStorage));
	photoShooting	 = PhotoShootingRef(new PhotoShooting(settings, photoStorage));
	photoChoosing	 = PhotoChoosingRef(new PhotoChoosing(settings, photoStorage));	
	photoTemplate	 = PhotoTemplateRef(new PhotoTemplate(settings, photoStorage));
	photoSharing     = PhotoSharingRef(new PhotoSharing(settings,   photoStorage));

	initLocations();
	cameraSetup();

	chrome().init();
}

void Photobooth::start()
{
	console()<<"START PHOTOBOOOOOOTH!"<<endl;
	updateSignal = App::get()->getSignalUpdate().connect(bind(&Photobooth::update, this));

	index = 0;
	currentLocation = locations[index];
	initShowAnimation();	
}

void Photobooth::showAnimationComplete()
{
	console() << "showAnimationComplete" << endl;
	state = DRAW;

	for (auto loc : locations)
	{
		loc->connectEventHandler(&Photobooth::nextLocationHandler, this, IPhotoboothLocation::NEXT_LOC);
		loc->connectEventHandler(&Photobooth::beginAnimHandler, this, IPhotoboothLocation::BEGIN_ANIM);
		loc->connectEventHandler(&Photobooth::completeAnimHandler, this, IPhotoboothLocation::COMPLETE_ANIM);
		loc->connectEventHandler(&Photobooth::disableGameCloseHandler, this, IPhotoboothLocation::DISABLE_GAME_CLOSE);
		loc->connectEventHandler(&Photobooth::enableGameCloseHandler, this, IPhotoboothLocation::ENABLE_GAME_CLOSE);
	}		

	photoChoosing->connectEventHandler(&Photobooth::reshotHandler, this, PhotoChoosing::RESHOT_LOC);
	currentLocation->start();
	callback(ENABLE_GAME_CLOSE);
}

void Photobooth::beginAnimHandler()
{
	console() << "DISABLE_GAME_CLOSE!" << endl;
	callback(DISABLE_GAME_CLOSE);
}

void Photobooth::completeAnimHandler()
{	
	console() << "ENABLE_GAME_CLOSE!" << endl;
	callback(ENABLE_GAME_CLOSE);
}

void Photobooth::enableGameCloseHandler()
{
	callback(ENABLE_GAME_CLOSE);
}

void Photobooth::disableGameCloseHandler()
{
	callback(DISABLE_GAME_CLOSE);
}

void Photobooth::stop()
{
	console()<<"STOP PHOTOBOOOOOOTH!"<<endl;
	updateSignal.disconnect();
	for (auto it: locations)
		it->stop();

	animX.stop();
	animX1.stop();
	alpha.stop();
}

void Photobooth::reset()
{
	console()<<"RESET PHOTOBOOOOOOTH!"<<endl;
	for (auto it: locations)
		it->reset(settings);
}

void Photobooth::initLocations()
{
	removeListeners();

	locations.clear();
	locations.push_back(photoInstruction);

	//locations.push_back(photoFilter);
	//locations.push_back(photoTimer);
	//locations.push_back(photoShooting);
	//locations.push_back(photoChoosing);
	//locations.push_back(photoTemplate);	
	locations.push_back(photoSharing);
}

void Photobooth::cameraSetup()
{
	cameraCanon().setup();
	cameraCanon().startLiveView();
	cameraCanon().setDownloadDirectory(settings->getPhotoDownloadDirectory());
	cameraCanon().setImageQuality();
}

void Photobooth::nextLocationHandler()
{
	if (++index >= locations.size())
		gotoFirstlocation();
	else
	{
		currentLocation = locations[index];
		currentLocation->start();
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
	currentLocation->update();
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
	}	
}

void Photobooth::removeListeners()
{
	for (auto loc : locations)
	{
		loc->disconnectEventHandler(IPhotoboothLocation::NEXT_LOC);
		loc->disconnectEventHandler(IPhotoboothLocation::BEGIN_ANIM);
		loc->disconnectEventHandler(IPhotoboothLocation::COMPLETE_ANIM);
		loc->disconnectEventHandler(IPhotoboothLocation::DISABLE_GAME_CLOSE);
		loc->disconnectEventHandler(IPhotoboothLocation::ENABLE_GAME_CLOSE);		
	}		
	photoChoosing->disconnectEventHandler(PhotoChoosing::RESHOT_LOC);
}