#include "Funces.h"

using namespace kubik;
using namespace kubik::games::funces;

Funces::Funces(ISettingsRef config)
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
}

void Funces::init(ISettingsRef config)
{
	settings = static_pointer_cast<FuncesSettings>(config);
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