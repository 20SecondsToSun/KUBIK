#include "Funces.h"

using namespace kubik;

Funces::Funces(shared_ptr<ISettings> config)
{	
	console()<<":: FUNCES CREATED::"<<endl;
	init(config);
	setType(ScreenId::FUNCES);
}

Funces::~Funces()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Funces destructor~~~~~~~~~~~~~~~~~~~"<<endl;	
}

void Funces::start()
{
//	addMouseUpListener();
}

void Funces::stop()
{
//	removeMouseUpListener();	
}

void Funces::init(ISettingsRef config)
{
	settings = static_pointer_cast<FuncesSettings>(config);
	
	//closeImg = settings->getTexture("closeImg");	
	//closeBtn = ButtonRef(new Button(closeImg, Vec2f(getWindowWidth() - 100.0f, 100.0f)));	
	//connect_once(closeBtn->mouseUpSignal, bind(&Funces::closeMouseUpHandler, this, std::placeholders::_1));
	//displayList.push_back(closeBtn);
}

void Funces::reset() 
{
	
}

void Funces::closeMouseUpHandler(IButton& button )
{	
	closeLocationSignal();
}

void Funces::draw()
{
	//closeBtn->draw();
}