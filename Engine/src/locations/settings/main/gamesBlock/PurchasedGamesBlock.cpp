#include "main/gamesBlock/PurchasedGamesBlock.h"

using namespace kubik;
using namespace kubik::config;

PurchasedGamesBlock::PurchasedGamesBlock(ConfigSettingsRef configSett, const vector<GamesInfo>& games)
	:oneGamePurchasedHeight(186),
	purchasedGamesSize(games.size())
{
	int i = 0;
	for (auto gameInfo : games)
	{
		OneGamePurchasedRef game = OneGamePurchasedRef(new OneGamePurchased(configSett, gameInfo));
		game->setPosition(Vec2f(0.0f, oneGamePurchasedHeight * i++));
		addChild(game);
	}
}

void PurchasedGamesBlock::activateListeners()
{
	for (auto game : displayList)
		game->activateListeners();//addMouseUpListener(&PurchasedGamesBlock::mouseUpFunction, this);						
}

float PurchasedGamesBlock::getHeight() const
{
	return oneGamePurchasedHeight * purchasedGamesSize;
}