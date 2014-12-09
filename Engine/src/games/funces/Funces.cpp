#include "Funces.h"

using namespace kubik;

Funces::Funces()
{	

}

Funces::~Funces()
{	
	console()<<"Funces destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();

	delete closeBtn;
	designTexures.clear();
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
	console()<<"casted"<<endl;
	closeImg = settings->getTextures()["closeImg"]->tex;	
	console()<<"created"<<endl;
	closeBtn = new Button(closeImg, Vec2f(getWindowWidth() - 100, 100));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&Funces::mouseUpHandler, this, std::placeholders::_1));
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