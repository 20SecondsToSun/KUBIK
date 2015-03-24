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
	console() << "START FUNCES!!!" << endl;
	initShowAnimation();
}

void Funces::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);
}

void Funces::stop()
{
	console() << "STOP FUNCES!!!" << endl;
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
	switch (state)
	{
	case SHOW_ANIM:
		screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		gl::color(Color(1, 0, 0));
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
		//currentLocation->draw();
		gl::popMatrices();
		break;

	case DRAW:
		//currentLocation->draw();
		break;
	}
}