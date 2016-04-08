#include "main/logo/Logo.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

Logo::Logo(ConfigSettingsRef configSettings, const Vec2i& position)
	:bckgrndColor(ci::Color::hex(0x242135)),
	blockWidth(245),
	blockHeight(170),
	isMiniState(false)
{
	setIcon(configSettings->getTexture("logoIcon"));
	setPosition(position);
}

void Logo::drawLayout()
{
	gl::color(bckgrndColor);
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(blockWidth, blockHeight)));
	gl::color(Color::white());
	gl::draw(icon, iconPosition);

	if (isMiniState)
	{
		gl::translate(0.5f * gameIcon.getSize());
		gl::scale(iconScale, iconScale);
		gl::translate(-0.5f * iconScale * gameIcon.getSize());
		gl::translate(0.0f, -123.0f);
		gl::draw(gameIcon);
	}
}

void Logo::setIcon(const gl::Texture& tex)
{
	icon = tex;
	saveIconPosition =
		iconPosition = Vec2f(0.5f * (blockWidth - icon.getWidth()),
		0.5f * (blockHeight - icon.getHeight()));
}

float Logo::getWidth() const
{
	return blockWidth;
}

void Logo::animateToMiniState(const gl::Texture& icon, const EaseFn& eFunc, float time)
{
	bckgrndColor = Color::hex(0x0d0917);
	iconPosition = saveIconPosition + Vec2f(-37.0f, 0.0f);
	isMiniState = true;
	gameIcon = icon;
	iconScale = 100.0f / gameIcon.getWidth();
}

void Logo::animateToMaxState(const EaseFn& eFunc, float time)
{
	bckgrndColor = Color::hex(0x242135);
	iconPosition = saveIconPosition;
	isMiniState = false;
}

void Logo::animateToUpState(const EaseFn& eFunc, float time)
{
	animatePosition = _localPosition;
	timeline().apply(&animatePosition, _localPosition + Vec2f(0.0f, -400.0f), time, eFunc)
		.updateFn(bind(&Logo::posAnimationUpdate, this));
}

void Logo::animateToDownState(const EaseFn& eFunc, float time)
{
	animatePosition = _localPosition;
	timeline().apply(&animatePosition, _localPosition + Vec2f(0.0f, 400.0f), time, eFunc)
		.updateFn(bind(&Logo::posAnimationUpdate, this));
}

void Logo::posAnimationUpdate()
{
	setPosition(animatePosition.value());
}