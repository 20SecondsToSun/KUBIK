#include "Photobooth.h"

Photobooth::Photobooth(ISettings* config)
{	
	console()<<"ISettings config::: "<<config<<endl;

	settings = static_cast<PhotoboothSettings*>(config);

	console()<<"PHOTOBOOTH SETTINGS::: "<<settings->getSeconds()<<endl;

	setTextures();	
}

Photobooth::~Photobooth()
{	
	console()<<"~~~~~~~~~~~~~~~ Photobooth destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();	
	designTexures.clear();
}

void Photobooth::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&Photobooth::mouseUp, this);
}

void Photobooth::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void Photobooth::setTextures()
{
	string mainFolder = getAppPath().string() + "data\\design\\template1\\";
	string menuPath   = "gamesDesign\\funces\\";	
	string path = mainFolder + menuPath;

	addToDictionary("closeImg",	path + "close.png",    resourceType::IMAGE, loadingType::FULL_PATH );
}

void Photobooth::init()
{	
	photoInstruction = shared_ptr<PhotoInstruction>(new PhotoInstruction());
	photoFilter		 = shared_ptr<PhotoFilter>(new PhotoFilter());
	photoTimer		 = shared_ptr<PhotoTimer>(new PhotoTimer());

	locations.push_back(photoInstruction);
	locations.push_back(photoFilter);
	locations.push_back(photoTimer);

	for (auto it: locations)	
		it->nextLocationSignal.connect(bind(&Photobooth::nextLocationHandler, this));

	currentLocation = locations.begin();

	closeImg = designTexures["closeImg"]->tex;
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100, 100)));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&Photobooth::mouseUpHandler, this, std::placeholders::_1));
}

void Photobooth::reset()
{
	for (auto it: locations)	
		it->reset();

	currentLocation = locations.begin();
}

void Photobooth::nextLocationHandler()
{	
	if(++currentLocation == locations.end())
		currentLocation = locations.begin();
}

void Photobooth::mouseUp( MouseEvent &event)
{	
	console()<<"Photobooth main mouse event::"<<endl;
	closeBtn->mouseUpHandler(event.getPos());
	(*currentLocation)->mouseUpHandler(event.getPos());
}

void Photobooth::mouseUpHandler(Button& button )
{	
	console()<<"Photobooth close event::"<<endl;
	closeGameSignal();
}

void Photobooth::draw()
{	
	(*currentLocation)->draw();
	closeBtn->draw();
}