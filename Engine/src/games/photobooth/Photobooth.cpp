#include "Photobooth.h"

using namespace kubik;
using namespace kubik::games;
using namespace std;
using namespace ci;
using namespace ci::app;

Photobooth::Photobooth(ISettingsRef config)
{
	console()<<"Photobooth CREATED::: "<<endl;
	init(config);
	create();
	setType(ScreenId::PHOTOBOOTH);
}

Photobooth::~Photobooth()
{	
	console()<<"~~~~~~~~~~~~~~~ Photobooth destructor ~~~~~~~~~~~~~~~"<<endl;
	updateSignal.disconnect();
	
	for (auto loc: locations)	
		loc->disconnectEventHandler(IPhotoboothLocation::NEXT_LOC);

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
	photoFilter		 = PhotoFilterRef(new PhotoFilter(settings));
	photoTimer		 = PhotoTimerRef(new PhotoTimer(settings));
	photoShooting	 = PhotoShootingRef(new PhotoShooting(settings, photoStorage));
	photoChoosing	 = PhotoChoosingRef(new PhotoChoosing(settings, photoStorage));	
	photoTemplate	 = PhotoTemplateRef(new PhotoTemplate(settings, photoStorage));
	//photoSharing     = PhotoSharingRef(new PhotoSharing(settings,	photoStorage));
	initLocations();
	
	cameraCanon().setup();
	cameraCanon().startLiveView();
}

void Photobooth::start()
{
	console()<<"START PHOTOBOOOOOOTH!"<<endl;
	updateSignal = App::get()->getSignalUpdate().connect(bind(&Photobooth::update, this));	

	currentLocation = locations.begin();
	(*currentLocation)->start();

	reset();
}

void Photobooth::stop()
{
	console()<<"STOP PHOTOBOOOOOOTH!"<<endl;
	for (auto it: locations)
		it->stop();
}

void Photobooth::reset()
{
	console()<<"RESET PHOTOBOOOOOOTH!"<<endl;
	//initLocations();

	for (auto it: locations)
		it->reset(settings);
}

void Photobooth::initLocations()
{
	for (auto loc: locations)	
		loc->disconnectEventHandler(IPhotoboothLocation::NEXT_LOC);

	locations.clear();
	locations.push_back(photoInstruction);
	locations.push_back(photoFilter);
	//locations.push_back(photoTimer);
	//locations.push_back(photoShooting);
	locations.push_back(photoChoosing);
	locations.push_back(photoTemplate);	
	//locations.push_back(photoSharing);

	for (auto loc: locations)	
		loc->connectEventHandler(&Photobooth::nextLocationHandler, this, IPhotoboothLocation::NEXT_LOC);
	
	currentLocation = locations.begin();
}	

void Photobooth::nextLocationHandler()
{
	(*currentLocation)->stop();

	if(++currentLocation == locations.end())	
		currentLocation = locations.begin();

	(*currentLocation)->start();
}

void Photobooth::update()
{
	(*currentLocation)->update();
}

void Photobooth::draw()
{	
	(*currentLocation)->draw();	
}