#include "main/gamesBlock/OneGamePurchased.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

OneGamePurchased::OneGamePurchased(ConfigSettingsRef config, const GamesInfo& info)
	:nameColor(Color::hex(0xffffff)),
	nameText(info.getNameText()),
	nameFont(getFont("IntroLight", 44)),
	checkerArea(Rectf(20.0f, 4.0f, 214.0f, 126.0f)),
	namePosition(Vec2f(282.0f, -10.0f)),
	info(info)
{
	toolfield = ToolFieldRef(new ToolField(config, info));
	addChild(toolfield);

	checker = GameCheckerRef(new GameChecker(checkerArea, info));
	addChild(checker);
}

void OneGamePurchased::drawLayout()
{
	gl::color(Color::white());
	textTools().textFieldDraw(nameText, &nameFont, nameColor, namePosition);
	Sprite::drawLayout();
}

void OneGamePurchased::setAlpha(float alpha)
{
	nameColor = Utils::colorAlpha(nameColor, alpha);
	Sprite::setAlpha(alpha);
}

void OneGamePurchased::activateListeners()
{
	checker->connectEventHandler(&OneGamePurchased::checkerClicked, this);
	toolfield->activateListeners();
}

void OneGamePurchased::unActivateListeners()
{
	toolfield->unActivateListeners();
	checker->disconnectEventHandler();
}

void OneGamePurchased::checkerClicked(EventGUIRef& event)
{	
	toolfield->swapActive();
	mouseUpSignal(event);
}

void OneGamePurchased::freeze()
{
	toolfield->setActive(true);
	checker->setFreeze();
}

void OneGamePurchased::unFreeze()
{
	if (checker->getValue())// unfreeze if was active only
		checker->setActive(true);
}

GameId OneGamePurchased::getGameID()
{
	return info.getGameId();
}