#include "PrinterBlock.h"
using namespace kubik;
using namespace kubik::config;

PrinterBlock::PrinterBlock(ConfigSettingsRef configSettings, const ci::Vec2i& position)
	:Sprite(),
	hintText(configSettings->getTextItem(ConfigTextID::PHOTO_LEFT)),
	changeBtnText(configSettings->getTextItem(ConfigTextID::CATRINGE_CHANGED)),
	iconColor(ci::Color::hex(0xffffff)),
	numsColor(ci::Color::hex(0xffffff)),
	bckgrndColor(ci::Color::hex(0x1a1827)),
	barColor1(ci::Color::hex(0x233442)),
	barColor2(ci::Color::hex(0x00f067)),
	maxBarWidth(312.0f),
	numsFont(configSettings->getFont("introBold72")),
	icon(configSettings->getTexture("catridgeIcon")),
	maxPhotosToPrint(configSettings->getData().maxPhotosToPrint)
{
	setPosition(position);

	ci::gl::Texture img = textTools().getTextField(changeBtnText);
	openBtn = ImageButtonSpriteRef(new ImageButtonSprite(img, Vec2f(670.0f, 62.5f)));
	addChild(openBtn);

	controls = PrinterControlsRef(new PrinterControls(configSettings, Vec2f(0.0f, 170.0f)));
	addChild(controls);

	hintTextTexture = textTools().getTextField(hintText);
	hintTextTexturePos = Vec2f(240.0f, 42.5);
}

void PrinterBlock::activateListeners()
{
	openBtn->connectEventHandler(&PrinterBlock::openButtonHandler, this);
}

void PrinterBlock::unActivateListeners()
{
	openBtn->disconnectEventHandler();
}

void PrinterBlock::openButtonHandler(EventGUIRef& event)
{
	if (eventHandlerDic[OPEN_EVENT])
	{
		openBtn->disconnectEventHandler();
		eventHandlerDic[OPEN_EVENT]();
	}
}

void PrinterBlock::drawLayout()
{
	gl::color(bckgrndColor);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, 835.0f, 170.0f));
	gl::color(Color::white());
	
	gl::draw(digitTexture, digitTexturePos);
	gl::draw(hintTextTexture, hintTextTexturePos);	

	gl::color(iconColor);
	gl::draw(icon, Vec2f(620.0f, 66.0f));
	gl::color(barColor1);
	gl::drawSolidRoundedRect(Rectf(245.0f, 90.0f, 245.0f + maxBarWidth, 103.0f), 8, 200);
	
	if (curBarWidth > 20)// good parametr for rounded rectangle
	{
		gl::color(barColor2);
		gl::drawSolidRoundedRect(Rectf(245.0f, 90.0f, 245.0f + curBarWidth, 103.0f), 8, 200);
		gl::color(Color::white());
	}
}

void PrinterBlock::draw()
{
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	drawLayout();
	gl::popMatrices();

	openBtn->draw();
	controls->draw();
}

void PrinterBlock::openControls(ci::EaseFn eFunc, float time)
{
	animatePosition = _localPosition;
	timeline().apply(&animatePosition, _localPosition + Vec2f(0.0f, -400.0f), time, eFunc)
		.updateFn(bind(&PrinterBlock::posAnimationUpdate, this))
		.finishFn(bind(&PrinterBlock::openControlsAnimationFinish, this));
}

void PrinterBlock::closeControls(ci::EaseFn eFunc, float time)
{
	animatePosition = _localPosition;
	timeline().apply(&animatePosition, _localPosition + Vec2f(0.0f, 400.0f), time, eFunc)
		.updateFn(bind(&PrinterBlock::posAnimationUpdate, this))
		.finishFn(bind(&PrinterBlock::hideControlsAnimationFinish, this));
}

void PrinterBlock::posAnimationUpdate()
{
	setPosition(animatePosition.value());
}

void PrinterBlock::hideControlsAnimationFinish()
{
	controls->unActivateListeners();
	controls->disconnectEventHandler();
	eventHandlerDic[HIDED]();
}

void PrinterBlock::openControlsAnimationFinish()
{
	if (eventHandlerDic[OPENED])
	{
		eventHandlerDic[OPENED]();
		controls->activateListeners();
		controls->connectEventHandler(&PrinterBlock::eventListener, this);
	}
}
void PrinterBlock::eventListener(EventGUIRef event)
{
	EventGUI *ev = event.get();
	if (!ev) return;

	if (typeid(*ev) == typeid(PrinterControlsHideEvent))
	{
		if (eventHandlerDic[HIDE_EVENT])
			eventHandlerDic[HIDE_EVENT]();
	}
	else if (typeid(*ev) == typeid(PrinterStatResetEvent))
	{
		if (eventHandlerDic[STAT_RESET_EVENT])
			eventHandlerDic[STAT_RESET_EVENT]();
	}
}

void PrinterBlock::setAlpha(float alpha)
{
	bckgrndColor = Utils::colorAlpha(bckgrndColor, alpha);
	numsColor = Utils::colorAlpha(numsColor, alpha);
	hintText.setColor(Utils::colorAlpha(hintText.getColor(), alpha));
	barColor1 = Utils::colorAlpha(barColor1, alpha);
	barColor2 = Utils::colorAlpha(barColor2, alpha);
	iconColor = Utils::colorAlpha(iconColor, alpha);
	Sprite::setAlpha(alpha);
}

void PrinterBlock::setMaxPhotosToPrint(int value)
{
	maxPhotosToPrint = value;
}

void PrinterBlock::set—urrentPhotosPrinted(int value)
{
	currentPhotosPrinted = value;
	curBarWidth = ((float)currentPhotosPrinted / maxPhotosToPrint) * maxBarWidth;

	digitTexture = textTools().getTextField(to_string(currentPhotosPrinted), &numsFont, numsColor);
	digitTexturePos = Vec2f(230.0f - digitTexture.getWidth(), 23.0f);
}