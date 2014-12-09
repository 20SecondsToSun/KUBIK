#include "Funces.h"

using namespace kubik;

Funces::Funces()
{	

}

Funces::~Funces()
{	
	mouseUpListener.disconnect();
	closeBtn->mouseUpSignal.disconnect_all_slots();
	console()<<"Funces destructor"<<endl;
}

void Funces::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&Funces::mouseUp, this);
}

void Funces::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void Funces::reset()
{
	
}

void Funces::init(ISettings* config)
{
	console()<<"::funces createTextures::  "<<endl;

	settings = static_cast<FuncesSettings*>(config);
	
	closeImg = settings->getTextures()["closeImg"]->tex;	
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100, 100)));	
	connect_once(closeBtn->mouseUpSignal, bind(&Funces::mouseUpHandler, this, std::placeholders::_1));
}

void Funces::mouseUp( MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
}

void Funces::mouseUpHandler(Button& button )
{	
	closeGameSignal();
}

void Funces::draw()
{	
	closeBtn->draw();
}