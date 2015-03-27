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
		mainGamesVec.push_back(game);
		addChild(game);
	}
}

void PurchasedGamesBlock::activateListeners()
{
	for (auto game : displayList)
		game->activateListeners();						
}

void PurchasedGamesBlock::unActivateListeners()
{
	for (auto game : displayList)
		game->unActivateListeners();	
}

void PurchasedGamesBlock::freezeChecker(const GameId& id)
{
	for (size_t i = 0; i < mainGamesVec.size(); i++)		
		if (id == mainGamesVec[i]->getGameID())		
			mainGamesVec[i]->freeze();		
}

void PurchasedGamesBlock::unFreezeChecker()
{
	for (size_t i = 0; i < mainGamesVec.size(); i++)
			mainGamesVec[i]->unFreeze();
}

float PurchasedGamesBlock::getHeight() const
{
	return oneGamePurchasedHeight * purchasedGamesSize;
}