#include "Photobooth.h"

using namespace kubik;

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

	mouseUpListener.disconnect();
	updateSignal.disconnect();

	closeBtn->mouseUpSignal.disconnect_all_slots();

	for (auto it: locations)	
		it->nextLocationSignal.disconnect_all_slots();

	locations.clear();
}

void Photobooth::init(ISettingsRef config)
{
	settings = static_pointer_cast<PhotoboothSettings>(config);	
}

void Photobooth::reset()
{
	initLocations();

	for (auto it: locations)
		it->reset(settings);
}

void Photobooth::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&Photobooth::mouseUp, this);
}

void Photobooth::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void Photobooth::create()
{	
	console()<<"CREATION::: "<<endl;
	photoStorage	 = PhotoStorageRef(new PhotoStorage());

	photoInstruction = PhotoInstructionRef(new PhotoInstruction(settings));
	photoFilter		 = PhotoFilterRef(new PhotoFilter(settings));
	photoTimer		 = PhotoTimerRef(new PhotoTimer(settings));
	photoShooting	 = PhotoShootingRef(new PhotoShooting(settings, photoStorage));
	photoProcessing	 = PhotoProcessingRef(new PhotoProcessing(settings, photoStorage));
	photoChoosing	 = PhotoChoosingRef(new PhotoChoosing(settings, photoStorage));
	photoTemplate	 = PhotoTemplateRef(new PhotoTemplate(settings, photoStorage));
	photoSharing     = PhotoSharingRef(new PhotoSharing(settings,	photoStorage));
	initLocations();	

	Texture closeImg = settings->getTextures()["closeImg"]->get();
	closeBtn = ButtonRef(new Button(closeImg, Vec2f(getWindowWidth() - 100.0f, 100.0f)));		
	connect_once(closeBtn->mouseUpSignal, bind(&Photobooth::mouseUpHandler, this, placeholders::_1));
	
	cameraCanon().setup();
	cameraCanon().startLiveView();
}

void Photobooth::start()
{
	console()<<"STARTING::: "<<endl;
	addMouseUpListener();	
	updateSignal = App::get()->getSignalUpdate().connect(bind(&Photobooth::update, this));	
	currentLocation = locations.begin();
	(*currentLocation)->start();	
}

void Photobooth::stop()
{
	console()<<"STOPPING::: "<<endl;
	removeMouseUpListener();	
}

void Photobooth::initLocations()
{
	for (auto it: locations)	
		it->nextLocationSignal.disconnect_all_slots();

	locations.clear();
	locations.push_back(photoInstruction);
	locations.push_back(photoFilter);
	locations.push_back(photoTimer);
	locations.push_back(photoShooting);
	locations.push_back(photoProcessing);	
	locations.push_back(photoChoosing);
	locations.push_back(photoTemplate);
	locations.push_back(photoSharing);

	for (auto it: locations)	
		connect_once(it->nextLocationSignal, bind(&Photobooth::nextLocationHandler, this));

	currentLocation = locations.begin();
}
	

void Photobooth::nextLocationHandler()
{
	if(++currentLocation == locations.end())	
		currentLocation = locations.begin();

	(*currentLocation)->start();
}

void Photobooth::mouseUp(MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
	(*currentLocation)->mouseUpHandler(event.getPos());

}

void Photobooth::mouseUpHandler(Button&button)
{	
	updateSignal.disconnect();
	closeLocationSignal();
}

void Photobooth::update()
{
	(*currentLocation)->update();
}

void Photobooth::draw()
{	
	(*currentLocation)->draw();
	closeBtn->draw();
}