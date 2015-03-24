#include "main/gamesBlock/GamesBlock.h"

using namespace kubik;
using namespace kubik::config;

GamesBlock::GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett, const Vec2f& position)
	:Sprite(),
	lineWidth(2),
	lineLength(880.0f),
	blockTopShiftY(79.0f),
	lineColor(Color::hex(0x233442)),
	configSett(configSett),
	gameSett(gameSett)
{
	setPosition(position);

	pGameblock = PurchasedGamesBlockRef(new PurchasedGamesBlock(configSett, gameSett->getData().getPurchasedGames()));
	pGameblock->setPosition(Vec2f(0.0f, blockTopShiftY));
	addChild(pGameblock);

	hasGamesInShop = gameSett->getData().getNotPurchasedGames().size() != 0;

	if (hasGamesInShop)
	{
		npGameblock = NotPurchasedGamesBlockRef(new NotPurchasedGamesBlock(configSett, gameSett->getData().getNotPurchasedGames()));
		npGameblock->setPosition(Vec2f(0.0f, pGameblock->getHeight() + 104.f));
		addChild(npGameblock);
	}
}

void GamesBlock::activateListeners()
{
	if (hasGamesInShop) 
		npGameblock->activateListeners();

	pGameblock->activateListeners();
}

void GamesBlock::draw()
{
	drawbg();
	drawDecorationLine1();
	Sprite::draw();

	if (hasGamesInShop)
		drawDecorationLine2();
}

void GamesBlock::drawbg()
{
	float height = pGameblock->getHeight() + 27.0f;
	gl::color(Color::hex(0x0d0917));
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(880.0f, height)));
	gl::popMatrices();
	gl::color(Color::white());
}

void GamesBlock::drawDecorationLine1()
{
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::lineWidth(lineWidth);
	gl::color(lineColor);
	gl::drawLine(Vec2f::zero(), Vec2f(lineLength, 0.0f));
	gl::popMatrices();
}

void GamesBlock::drawDecorationLine2()
{
	float height = pGameblock->getHeight() + 27.0f;
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::lineWidth(lineWidth);
	gl::color(lineColor);
	gl::translate(0.0f, blockTopShiftY);
	gl::drawLine(Vec2f(0.0f, height), Vec2f(lineLength, height));
	gl::popMatrices();
}

void GamesBlock::setAlpha(float alpha)
{
	lineColor = Utils::colorAlpha(lineColor, alpha);
	Sprite::setAlpha(alpha);
}

void GamesBlock::hide(const EaseFn& eFunc, float time)
{
	animatePosition = _localPosition;
	Vec2f finPos = Vec2f(-getWindowWidth(), _localPosition.y);
	timeline().apply(&animatePosition, finPos, time, eFunc)
		.updateFn(bind(&GamesBlock::posAnimationUpdate, this))
		.finishFn(bind(&GamesBlock::hideAnimationFinish, this));
}

void GamesBlock::hideAnimationFinish()
{
	hideAnimCompleteSig();
}

void GamesBlock::show(const EaseFn& eFunc, float time)
{
	Vec2f finPos = Vec2f(100.0f, _localPosition.y);

	timeline().apply(&animatePosition, finPos, time, eFunc)
		.updateFn(bind(&GamesBlock::posAnimationUpdate, this))
		.finishFn(bind(&GamesBlock::showAnimationFinish, this));
}

void GamesBlock::showAnimationFinish()
{
	showAnimCompleteSig();
}

void GamesBlock::posAnimationUpdate()
{
	setPosition(animatePosition.value());
}