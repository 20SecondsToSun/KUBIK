#include "NewActivityPopup.h"
using namespace ci;
using namespace kubik;
using namespace kubik::config;

NewActivityPopup::NewActivityPopup(ConfigSettingsRef configSett)
	:Sprite(),
	configSett(configSett),
	bgColor(Color::hex(0x2f3643)),
	headColor(Color::hex(0x424d5f)),
	titlesColor(ColorA::white()),
	inputFieldColor(Color::hex(0xe5e9f2)),
	btnStartColor(Color::hex(0x6798ff)),
	closeIcon(configSett->getTexture("closePrinterIcon")),
	inputField(configSett->getTexture("inputField")),
	redFocus(configSett->getTexture("redFocus")),
	redFocusAlpha(0.0f)

{
	mainTitle = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_ASK_TITLE), true, -10);
	subTitle = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_DESQR), true, -8);
	title = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_TITLE));
	//begin = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_BEGIN));
	closeTitle = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_CLOSE));
	float allWidth = closeIcon.getWidth() + 20.0f + closeTitle.getWidth();
	float startX = 0.5f * (getWindowWidth() - allWidth);

	closeBtnPos = Vec2f(startX, 892.0f);// 100.0f);
	CloseActivityEventRef  closeEvent = CloseActivityEventRef(new CloseActivityEvent());
	closeBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(closeBtnPos, closeBtnPos + Vec2f(allWidth, 40.0f)), closeEvent));

	yPositionInputField = 703.0f; //200.0f;//
	inputFieldPos = Vec2f(0.5f * (1080.0f - inputField.getWidth()), yPositionInputField);
	StartNewActivityEventRef  startEvent = StartNewActivityEventRef(new StartNewActivityEvent());
	startBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(750.0f, yPositionInputField, 962.0f, yPositionInputField + 140.0f), startEvent));
}

void NewActivityPopup::draw()
{
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	drawBackgrounds();
	drawTitles();
	drawInputFieldBackground();
	drawCloseBlock();
	touchKeyboard().draw();
	gl::popMatrices();
	Sprite::draw();
}

void NewActivityPopup::drawBackgrounds()
{
	float height = 514.0f;
	gl::color(headColor);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), height));
	gl::color(bgColor);
	gl::pushMatrices();
	gl::translate(0.0f, height);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 1920.0f - height));
	gl::color(Color::white());
	gl::popMatrices();
}

void NewActivityPopup::drawTitles()
{
	gl::color(titlesColor);
	gl::draw(mainTitle, Vec2f(0.5f*(getWindowWidth() - mainTitle.getWidth()), 102.0f));
	gl::draw(subTitle, Vec2f(0.5f*(getWindowWidth() - subTitle.getWidth()), 312.0f));
	gl::draw(title, Vec2f(0.5f*(getWindowWidth() - title.getWidth()), 610.0f));
}

void NewActivityPopup::drawInputFieldBackground()
{
	/*
	gl::drawSolidRoundedRect(ci::Rectf(130, 932, 130 + 828, 932 + 140), 7);
	gl::color(btnStartColor);
	gl::drawSolidRoundedRect(ci::Rectf(752.0, 932.0, 962.0, 1072.0), 7);
	gl::drawSolidRect(ci::Rectf(750.0f, 932.0, 760.0, 1072.0), 7);	*/
	gl::color(ColorA(1.0f, 1.0f, 1.0f, redFocusAlpha));
	gl::draw(redFocus, inputFieldPos - Vec2f(3.0f, 3.0f));

	gl::color(inputFieldColor);
	gl::draw(inputField, inputFieldPos);
	//gl::color(titlesColor);
	textTools().drawTextBox(configSett->getTextItem(ConfigTextID::PARTY_BEGIN), Color::hex(0x6595f9), titlesColor, Vec2f(750.0f, yPositionInputField), Vec2i(200.0f, 134.0f));

	//gl::draw(begin, Vec2f(750.0f + 0.5f * (210.0f - begin.getWidth()), yPositionInputField + 0.5f * (142.0f - begin.getHeight())));
}

void NewActivityPopup::drawCloseBlock()
{
	gl::color(titlesColor);
	gl::draw(closeIcon, closeBtnPos);
	gl::draw(closeTitle, closeBtnPos + Vec2f(20.0f + closeIcon.getWidth(), -1.0f));
}

void NewActivityPopup::setAlpha(float alpha)
{
	bgColor			= Utils::colorAlpha(bgColor, alpha);
	headColor		= Utils::colorAlpha(headColor, alpha);
	titlesColor		= Utils::colorAlpha(titlesColor, alpha);
	inputFieldColor = Utils::colorAlpha(titlesColor, alpha);
	btnStartColor	= Utils::colorAlpha(btnStartColor, alpha);
	Sprite::setAlpha(alpha);
}

void NewActivityPopup::show(const EaseFn& eFunc, float time)
{
	alpha = 0.0f;
	timeline().apply(&alpha, 0.97f, time, eFunc)
		.updateFn(bind(&NewActivityPopup::alphAnimationUpdate, this))
		.finishFn(bind(&NewActivityPopup::showAnimationFinish, this));

	touchKeyboard().clearInputFieldText();
	touchKeyboard().setInputField(inputFieldPos.x, inputFieldPos.y, inputFieldPos.x + 613.0f, inputFieldPos.y + 139.0f);

	auto endY = 1034.0f;
	touchKeyboard().show(Vec2f(30.0f, endY + 500.0f), Vec2f(30.0f, endY), 0.7f);
}

void NewActivityPopup::hide(const EaseFn& eFunc, float time)
{
	timeline().apply(&alpha, 0.0f, time, eFunc)
		.updateFn(bind(&NewActivityPopup::alphAnimationUpdate, this))
		.finishFn(bind(&NewActivityPopup::hideAnimationFinish, this));
}

std::string NewActivityPopup::getCompainName()
{
	return touchKeyboard().getInputFieldText();
}

void NewActivityPopup::alphAnimationUpdate()
{
	setAlpha(alpha.value());
}

void NewActivityPopup::showAnimationFinish()
{
	if (eventHandlerDic[OPENED])
	{
		eventHandlerDic[OPENED]();
		closeBtn->connectEventHandler(&NewActivityPopup::closeHandler, this);
		startBtn->connectEventHandler(&NewActivityPopup::newCompainHandler, this);
	}

	initVirtualKeyboard();
}

void NewActivityPopup::initVirtualKeyboard()
{
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().connectKeyboard();
	touchKeyboard().setInputFont(configSett->getFont("introLight44"));
	touchKeyboard().setInputColor(Color::black());	
}

void NewActivityPopup::closeHandler(EventGUIRef& event)
{
	if (eventHandlerDic[HIDE_EVENT])
	{
		disconnect();
		eventHandlerDic[HIDE_EVENT]();
	}
}

void NewActivityPopup::newCompainHandler(EventGUIRef& event)
{
	if (!touchKeyboard().emptyInputField())
	{
		if (eventHandlerDic[START_NEW_COMPAIN])
		{
			disconnect();
			eventHandlerDic[START_NEW_COMPAIN]();
		}
	}
	else
		showRedFocusStroke();
}

void NewActivityPopup::showRedFocusStroke()
{
	timeline().apply(&redFocusAlpha, 1.0f, 0.0f, 2.5f, EaseOutCubic());
}

void NewActivityPopup::disconnect()
{
	closeBtn->disconnectEventHandler();
	startBtn->disconnectEventHandler();

	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hide(Vec2f(30.0f, 1950.0f), 0.3f);
}

void NewActivityPopup::hideAnimationFinish()
{
	callback(HIDED);
}