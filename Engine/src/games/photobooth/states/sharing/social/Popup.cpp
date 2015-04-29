#include "states/sharing/social/Popup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

Popup::Popup(PhotoboothSettingsRef settings)
	:headColor(Color::hex(0x4b515a)),
	bgColor(Color::hex(0x34383f)), 
	closeIcon(settings->getTexture("closePopup")),
	inputFont(settings->getFont("introBook44"))	
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(closeIcon, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - closeIcon.getSize() * 0.5f));
}

void Popup::show()
{
	initVirtualKeyboard();

	timeline().apply(&alphaAnim, 0.0f, 0.97f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::showAnimComplete, this));
}

void Popup::initVirtualKeyboard()
{
	touchKeyboard().activateSendMode();
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().connectKeyboard();
	auto endY = 500.0f;//800.0f;
	touchKeyboard().show(Vec2f(30.0f, endY + 500.0f), Vec2f(30.0f, endY), 0.7f);
}

void Popup::showAnimComplete()
{
	connectCloseBtn();
}

void Popup::kill()
{
	disconnectCloseBtn();
}

void Popup::connectCloseBtn()
{
	closeBtn->connectEventHandler(&Popup::hide, this);
}

void Popup::disconnectCloseBtn()
{
	closeBtn->disconnectEventHandler();
}

void Popup::hideAnimComplete()
{
	callback(POPUP_CLOSED);
}

void Popup::hide(EventGUIRef& event)
{
	close();
}

void Popup::close()
{
	closeBtn->disconnectEventHandler();

	touchKeyboard().clearInputFieldText();
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hide(Vec2f(30.0f, 1950.0f), 0.3f);

	timeline().apply(&alphaAnim, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::hideAnimComplete, this));
}

void Popup::draw()
{
	drawBackgrounds();

	closeBtn->setAlpha(alphaAnim);
	closeBtn->draw();

	touchKeyboard().draw();

	gl::color(Color::white());
}

void Popup::drawBackgrounds()
{
	float height = 716.0f;
	auto color = Utils::colorAlpha(headColor, alphaAnim);
	gl::color(color);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), height));
	color = Utils::colorAlpha(bgColor, alphaAnim);
	gl::color(color);
	gl::pushMatrices();
	gl::translate(0.0f, height);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 1920.0f - height));	
	gl::popMatrices();
}