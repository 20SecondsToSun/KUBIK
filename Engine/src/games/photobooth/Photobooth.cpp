#include "Photobooth.h"

Photobooth::Photobooth()
{	
	setTextures();	
}

Photobooth::~Photobooth()
{	
	console()<<"Photobooth destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();
	delete closeBtn;
	designTexures.clear();
}

void Photobooth::addMouseUpListener()
{
	mouseUpListener = getWindow()->getSignalMouseUp().connect( std::bind( &Photobooth::mouseUp, this, std::placeholders::_1) );
}

void Photobooth::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void Photobooth::setTextures()
{
	addToDictionary("img1", "gamesDesign\\photobooth\\1.jpg");
	addToDictionary("closeImg", "gamesDesign\\photobooth\\close.png");
	addToDictionary("img3", "gamesDesign\\photobooth\\3.jpg");
}

void Photobooth::init()
{
	closeImg = designTexures["closeImg"]->tex;
	console()<<"::Photobooth createTextures::  "<<closeImg<<endl;
	closeBtn = new Button(closeImg, Vec2f(400,200));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&Photobooth::mouseUpHandler, this, std::placeholders::_1));
}

void Photobooth::mouseUp( MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
}

void Photobooth::mouseUpHandler(Button& button )
{	
	console()<<"Photobooth close event::"<<endl;
	closeGameSignal();
}

void Photobooth::draw()
{	
	closeBtn->draw();
}