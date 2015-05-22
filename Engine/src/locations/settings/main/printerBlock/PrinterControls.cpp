#include "PrinterControls.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

PrinterControls::PrinterControls(ConfigSettingsRef configSettings, const Vec2i& position)
	:Sprite(), configSettings(configSettings),
	titleText1(configSettings->getTextItem(ConfigTextID::PRINTER_ASK_TITLE)),
	titleText2(configSettings->getTextItem(ConfigTextID::PRINTER_ASK_SUB_TITLE)),
	iErrorText(configSettings->getTextItem(ConfigTextID::PRINTER_CLOSE_TITLE)),
	changedText(configSettings->getTextItem(ConfigTextID::PRINTER_OK_TITLE)),
	closeIcon(configSettings->getTexture("closePrinterIcon"))
{
	setPosition(position);
	okButtonArea = Rectf(632.0f, 230.0f, 917.0f, 300.0f);

	Texture tex = textTools().getTextField(changedText);
	okTextPos = Vec2f(okButtonArea.x1 + 5.0f, okButtonArea.y1 + 5.0f);

	PrinterStatResetEventRef  eventReset = PrinterStatResetEventRef(new PrinterStatResetEvent());
	okBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(okButtonArea, eventReset));
	okBtn->setAlpha(0.5f);
	addChild(okBtn);

	PrinterControlsHideEventRef eventHide = PrinterControlsHideEventRef(new PrinterControlsHideEvent());
	closeBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(415.0f, 40.0f, Vec2f(160.0f, 248.0f), eventHide));
	closeBtn->setAlpha(0.5f);
	addChild(closeBtn);

	closeBtnBig = SimpleSpriteButtonRef(new SimpleSpriteButton(getWindowWidth(), 1350.0f, Vec2f(0.0f, -1520.0f), eventHide));
	closeBtnBig->setAlpha(0.5f);
	addChild(closeBtnBig);
}

void PrinterControls::activateListeners()
{
	okBtn->connectEventHandler(&PrinterControls::eventListener, this);
	closeBtnBig->connectEventHandler(&PrinterControls::eventListener, this);
	closeBtn->connectEventHandler(&PrinterControls::eventListener, this);
}

void PrinterControls::unActivateListeners()
{
	closeBtnBig->disconnectEventHandler();
	closeBtn->disconnectEventHandler();
	okBtn->disconnectEventHandler();
}

void PrinterControls::drawLayout()
{
	gl::color(Color::hex(0x171521));
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 400.0f));
	gl::color(Color::white());
	textTools().textFieldDraw(titleText1, Vec2f(150.0f, 95.0f));
	textTools().textFieldDraw(titleText2, Vec2f(157.0f, 158.0f));

	textTools().textFieldDraw(iErrorText, Vec2f(202.0f, 246.0f));
	gl::draw(closeIcon, Vec2f(160.0f, 248.0f));

	gl::color(Color::hex(0x6798ff));
	gl::drawSolidRoundedRect(okButtonArea, 7);
	gl::color(Color::white());
	textTools().textFieldDraw(changedText, okTextPos);
	textTools().drawTextBox(changedText, Color::hex(0x6798ff), okTextPos, okBtn->getSize() - Vec2f(10.0f, 10.0f));
}

void PrinterControls::draw()
{
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	drawLayout();
	gl::popMatrices();
}