#include "main/designBlock/ReturnButton.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

ReturnButton::ReturnButton(ConfigSettingsRef configSettings, const Vec2i& position)
	:SimpleSpriteButton(Rectf(0.0f, 0.0f, 707.0f, 175.0f)),
	icon(configSettings->getTexture("circles")),
	text(configSettings->getTextItem(ConfigTextID::BACK))
{
	setPosition(position);
	event = CloseDesignLayoutEventRef(new CloseDesignLayoutEvent());

	textTexture = textTools().getTextField(text);
	float width = textTexture.getWidth() + icon.getWidth() + 28.0f;
	startX = 0.5f * (707.0f - width);
}

void ReturnButton::drawLayout()
{
	gl::color(text.getColor());
	gl::pushMatrices();
	gl::translate(startX, 95.0f);
	gl::draw(icon);
	gl::translate(icon.getWidth() + 28.0f, -11.0f);
	gl::draw(textTexture);
	gl::popMatrices();
	gl::color(Color::white());
}

void ReturnButton::setAlpha(float alpha)
{
	text.setColor(Utils::colorAlpha(text.getColor(), alpha));
}