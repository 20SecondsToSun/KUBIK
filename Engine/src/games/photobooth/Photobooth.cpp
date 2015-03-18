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
}

void Photobooth::start()
{
	console()<<"START PHOTOBOOOOOOTH!"<<endl;
	updateSignal = App::get()->getSignalUpdate().connect(bind(&Photobooth::update, this));	
	gotoFirstlocation();
	reset();
}

void Photobooth::stop()
{
	console()<<"STOP PHOTOBOOOOOOTH!"<<endl;
	updateSignal.disconnect();
	for (auto it: locations)
		it->stop();
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
	locations.push_back(photoFilter);
	locations.push_back(photoTimer);
	locations.push_back(photoShooting);
	locations.push_back(photoChoosing);
	locations.push_back(photoTemplate);	
	locations.push_back(photoSharing);

	for (auto loc: locations)	
		loc->connectEventHandler(&Photobooth::nextLocationHandler, this, IPhotoboothLocation::NEXT_LOC);

	photoChoosing->connectEventHandler(&Photobooth::reshotHandler, this, PhotoChoosing::RESHOT_LOC);
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
	//currentLocation->stop();

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
	currentLocation->draw();	
}

void Photobooth::removeListeners()
{
	for (auto loc: locations)	
		loc->disconnectEventHandler(IPhotoboothLocation::NEXT_LOC);

	photoChoosing->disconnectEventHandler(PhotoChoosing::RESHOT_LOC);
}