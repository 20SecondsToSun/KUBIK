#include "Instakub.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

Instakub::Instakub(ISettingsRef config)
{	
	console()<<":: Instakub CREATED::"<<endl;
	init(config);
	setType(ScreenId::INSTAKUB);
}

Instakub::~Instakub()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Instakub destructor~~~~~~~~~~~~~~~~~~~"<<endl;	
}

void Instakub::start()
{
	console() << "START Instakub!!!" << endl;
	initShowAnimation();
}

void Instakub::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);
}

void Instakub::stop()
{
	console() << "STOP Instakub!!!" << endl;
}

void Instakub::init(ISettingsRef config)
{
	settings = static_pointer_cast<InstakubSettings>(config);
}

void Instakub::reset()
{
	
}

void Instakub::closeMouseUpHandler(IButton& button )
{	
	closeLocationSignal();
}

void Instakub::draw()
{
	switch (state)
	{
	case SHOW_ANIM:
		screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		gl::color(Color(1, 1, 0));
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