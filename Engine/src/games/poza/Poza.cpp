#include "Poza.h"

using namespace kubik;
using namespace kubik::games::poza;

Poza::Poza(ISettingsRef config)
{
	logger().log("~~~ Poza.Created ~~~");

	init(config);
	setType(ScreenId::FUNCES);
}

Poza::~Poza()
{
	logger().log("~~~ Poza.Destruct ~~~");
}

void Poza::start()
{
	logger().log("~~~ Poza.Start ~~~");

	initShowAnimation();
}

void Poza::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);
}

void Poza::stop()
{
	logger().log("~~~ Poza.Stop ~~~");
}

void Poza::init(ISettingsRef config)
{
	settings = static_pointer_cast<PozaSettings>(config);
}

void Poza::reset()
{

}

void Poza::closeMouseUpHandler(IButton& button)
{
	closeLocationSignal();
}

void Poza::draw()
{
	switch (state)
	{
	case SHOW_ANIM:
		screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		gl::color(Color::black());
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
		gl::draw(settings->getTexture("title"), Vec2f(200.0f, 200.0f));
		//currentLocation->draw();
		gl::popMatrices();
		break;

	case DRAW:
		//currentLocation->draw();
		break;
	}
}