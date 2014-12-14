#include "Photobooth.h"

using namespace kubik;

Photobooth::Photobooth(shared_ptr<ISettings> config)
{
	console()<<"Photobooth CREATED::: "<<endl;

	init(config);
	create();
}

Photobooth::~Photobooth()
{	
	console()<<"~~~~~~~~~~~~~~~ Photobooth destructor ~~~~~~~~~~~~~~~"<<endl;
	mouseUpListener.disconnect();
	closeBtn->mouseUpSignal.disconnect_all_slots();

	for (auto it: locations)	
		it->nextLocationSignal.disconnect_all_slots();

	locations.clear();
}

void Photobooth::init(shared_ptr<ISettings> config)
{
	settings = static_pointer_cast<PhotoboothSettings>(config);	
}

void Photobooth::reset(shared_ptr<ISettings> config)
{
	settings = static_pointer_cast<PhotoboothSettings>(config);	

	for (auto it: locations)
		it->reset(settings);

	currentLocation = locations.begin();
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
	photoInstruction = shared_ptr<PhotoInstruction>(new PhotoInstruction(settings));
	photoFilter		 = shared_ptr<PhotoFilter>(new PhotoFilter(settings));
	photoTimer		 = shared_ptr<PhotoTimer>(new PhotoTimer(settings));

	locations.push_back(photoInstruction);
	locations.push_back(photoFilter);
	locations.push_back(photoTimer);


	for (auto it: locations)	
		connect_once(it->nextLocationSignal, bind(&Photobooth::nextLocationHandler, this));

	currentLocation = locations.begin();

	closeImg = settings->getTextures()["closeImg"]->get();
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100.0f, 100.0f)));		
	connect_once(closeBtn->mouseUpSignal, bind(&Photobooth::mouseUpHandler, this, std::placeholders::_1));
}

void Photobooth::nextLocationHandler()
{	
	if(++currentLocation == locations.end())
		currentLocation = locations.begin();
}

void Photobooth::mouseUp( MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
	(*currentLocation)->mouseUpHandler(event.getPos());
}

void Photobooth::mouseUpHandler(Button& button )
{	
	closeLocationSignal();
}

void Photobooth::draw()
{	
	(*currentLocation)->draw();
	closeBtn->draw();
}