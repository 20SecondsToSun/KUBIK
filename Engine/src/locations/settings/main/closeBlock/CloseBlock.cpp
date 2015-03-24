#include "main/closeBlock/CloseBlock.h"

using namespace kubik;
using namespace kubik::config;

CloseBlock::CloseBlock(ConfigSettingsRef configSettings, const ci::Vec2i& position)
	:ImageButtonSprite(configSettings->getTexture("iconClose"), position),
	iconClose(configSettings->getTexture("iconClose")),
	iconBack(configSettings->getTexture("iconBack"))
{
	event = closeEvent = CloseConfigEventRef(new CloseConfigEvent());
	backEvent = BackToMainConfigEventRef(new BackToMainConfigEvent());
	setPosition(position);
}

void CloseBlock::animateToMiniState(const ci::EaseFn& eFunc, float time, const ci::Vec2i&  finPos)
{
	setPosition(_localPosition - ci::Vec2f(40.0f, 0.0f));
	event = backEvent;

	changeTexture(iconBack);
}

void CloseBlock::animateToMaxState(const ci::EaseFn& eFunc, float time)
{
	setPosition(_localPosition + ci::Vec2f(40.0f, 0.0f));
	event = closeEvent;
	changeTexture(iconClose);
}
