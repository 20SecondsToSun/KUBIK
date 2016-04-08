#include "main/gamesBlock/OneGameNotPurchased.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

OneGameNotPurchased::OneGameNotPurchased(ConfigSettingsRef config, const GamesInfo& info)
	:nameColor(ci::Color::hex(0x939eb0)),
	SimpleSpriteButton(ci::Rectf(0.0f, 0.0f, 0.0f, 0.0f)),
	btnColor(ci::Color::white())
{
	nameText = info.getNameText();
	nameFont = getFont("IntroLight", 36);

	miniIcon = info.getMiniIcon();
	gameInWebIcon = config->getTexture("gameInWebIcon");

	textTexture = textTools().getTextField(nameText, &nameFont, nameColor);

	event = GameShowUrlEventRef(new GameShowUrlEvent(info.getGameId()));

	setButtonArea(ci::Rectf(0.0f, 0.0f, 112.0f + textTexture.getWidth() + gameInWebIcon.getWidth(), 65));
}

void OneGameNotPurchased::drawLayout()
{
	gl::color(btnColor);
	gl::pushMatrices();
	//gl::translate(position);
	gl::draw(miniIcon);
	gl::draw(textTexture, ci::Vec2f(91.0f, 2.0f));
	gl::draw(gameInWebIcon, ci::Vec2f(112.0f + textTexture.getWidth(), 13.0f));
	//gl::drawSolidRect(buttonArea);
	gl::popMatrices();
}

void OneGameNotPurchased::setAlpha(float alpha)
{
	btnColor  = ColorA(btnColor.r, btnColor.g, btnColor.b, alpha);
	nameColor = ColorA(nameColor.r, nameColor.g, nameColor.b, alpha);
}