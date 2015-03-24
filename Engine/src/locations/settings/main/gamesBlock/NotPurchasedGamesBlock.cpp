#include "main/gamesBlock/NotPurchasedGamesBlock.h"

using namespace kubik;
using namespace kubik::config;

NotPurchasedGamesBlock::NotPurchasedGamesBlock(ConfigSettingsRef configSett, const vector<GamesInfo>& games)
	:titleText(configSett->getTextItem(ConfigTextID::NOTINSTALL))
{
	for (auto gameInfo : games)
	{
		OneGameNotPurchasedRef pGame = OneGameNotPurchasedRef(new OneGameNotPurchased(configSett, gameInfo));
		addChild(pGame);
	}
}

void NotPurchasedGamesBlock::activateListeners()
{
	for (auto game : displayList)
		game->connectEventHandler(&NotPurchasedGamesBlock::mouseUpFunction, this);
}

void NotPurchasedGamesBlock::unActivateListeners()
{
	for (auto game : displayList)
		game->disconnectEventHandler();
}

void NotPurchasedGamesBlock::mouseUpFunction(EventGUIRef& event)
{
	console() << "mouseUpFunction" << endl;
	mouseUpSignal(event);
}

void NotPurchasedGamesBlock::drawLayout()
{
	gl::color(Color::white());
	textTools().textFieldDraw(titleText, Vec2f(-8.0f, 56.0f));
}

void NotPurchasedGamesBlock::setAlpha(float alpha)
{
	titleText.setColor(Utils::colorAlpha(titleText.getColor(), alpha));
	for (auto game : displayList)
		game->setAlpha(alpha);
}

void NotPurchasedGamesBlock::setPosition(ci::Vec2i position)
{
	int i = 0;
	for (auto game : displayList)
	{
		game->setPosition(Vec2f(445.0f * (i % 2), 56.0f + 86.0f + 120.0f * (i / 2)));
		i++;
	}

	Sprite::setPosition(position);
}