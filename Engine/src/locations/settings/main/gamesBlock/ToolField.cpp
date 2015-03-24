#include "main/gamesBlock/ToolField.h"

using namespace kubik;
using namespace kubik::config;

ToolField::ToolField(ConfigSettingsRef config, const GamesInfo& info)
	:Sprite(),
	offText(config->getTextItem(ConfigTextID::SWITCH_OFF))
{
	Rectf ba1 = Rectf(295.0f, 65.0f, 295.0f + 180, 65.0f + 55.0f);
	Rectf ba2 = Rectf(295.0f + 205, 65.0f, 295.0f + 205.0f + 180.0f + 10.0f, 65.0f + 55.0f);

	statBtn = StatToolButtonRef(new StatToolButton(ba1, info.getGameId(), config->getTextItem(ConfigTextID::STAT)));
	statBtn->setColor(Color::hex(0x6798ff));
	addChild(statBtn);

	toolBtn = ConfToolButtonRef(new ConfToolButton(ba2, info.getGameId(), config->getTextItem(ConfigTextID::CONFIG)));
	toolBtn->setColor(Color::hex(0x00f067));
	addChild(toolBtn);

	this->isActive = info.isGameOn();
}

void ToolField::draw()
{
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	if (!isActive)
		drawOffMessage();
	gl::popMatrices();

	if (isActive)
		Sprite::draw();
}

void ToolField::drawOffMessage()
{
	gl::color(Color::white());
	textTools().textFieldDraw(offText, Vec2f(289.0f, 55.0f));
}

void ToolField::setActive(bool isActive)
{
	this->isActive = isActive;

	if (isActive)
	{
		toolBtn->connectEventHandler(&ToolField::mouseUpFunction, this);
		statBtn->connectEventHandler(&ToolField::mouseUpFunction, this);
	}
	else
	{
		toolBtn->disconnectEventHandler();
		statBtn->disconnectEventHandler();
	}
}

void ToolField::unActivateListeners()
{
	toolBtn->disconnectEventHandler();
	statBtn->disconnectEventHandler();
}

void ToolField::mouseUpFunction(EventGUIRef& event)
{
	mouseUpSignal(event);
}

void ToolField::activateListeners()
{
	setActive(isActive);
}

void ToolField::setAlpha(float alpha)
{
	offText.setColor(Utils::colorAlpha(offText.getColor(), alpha));
	Sprite::setAlpha(alpha);
}

void ToolField::swapActive()
{
	setActive(!isActive);
}