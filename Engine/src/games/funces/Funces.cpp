#include "Funces.h"

using namespace kubik;

Funces::Funces(shared_ptr<ISettings> config)
{	
	console()<<":: FUNCES CREATED::"<<endl;
	init(config);
}

Funces::~Funces()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Funces destructor~~~~~~~~~~~~~~~~~~~"<<endl;
	mouseUpListener.disconnect();
	closeBtn->mouseUpSignal.disconnect_all_slots();
}

void Funces::init(shared_ptr<ISettings> config)
{
	settings = static_pointer_cast<FuncesSettings>(config);
	
	closeImg = settings->getTextures()["closeImg"]->tex;	
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100, 100)));	
	connect_once(closeBtn->mouseUpSignal, bind(&Funces::mouseUpHandler, this, std::placeholders::_1));
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

void Funces::mouseUp( MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
}

void Funces::mouseUpHandler(Button& button )
{	
	closeLocationSignal();
}

void Funces::draw()
{
	closeBtn->draw();
}