#include "states/sharing/social/TwitterPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

TwitterPopup::TwitterPopup(PhotoboothSettingsRef settings)
	:Popup(settings)
{
	twitter = TwitterRef(new Twitter());
	socialTexture = settings->getTexture("twitterPopupTexture");
	socialTexturePosition = Vec2f(.5f * (1080 - socialTexture.getWidth()), 393.0f - socialTexture.getHeight() * 0.5f);
}

void TwitterPopup::show()
{
	Popup::show();
}

void TwitterPopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void TwitterPopup::showAnimComplete()
{
	touchKeyboard().connectEventHandler(&TwitterPopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
	touchKeyboard().connectEventHandler(&TwitterPopup::sendHandler, this, VirtualKeyboard::SEND_TOUCH);

	connect_once(twitter->postingComplete, bind(&TwitterPopup::postingCompleteHandler, this));
	connect_once(twitter->postingError, bind(&TwitterPopup::postingErrorHandler, this));

	Popup::showAnimComplete();	
}

void TwitterPopup::postingCompleteHandler()
{
	disconnectListeners();
	Popup::hide(event);
}

void TwitterPopup::postingErrorHandler()
{
	disconnectListeners();
	Popup::hide(event);
}

void TwitterPopup::keyTouchHandler()
{
	//console() << "::::::touch " << touchKeyboard().getLastCode() << endl;	
}

void TwitterPopup::sendHandler()
{
	console() << "send!!!!!!!!!!" << endl;
	twitter->authorizePost("yurikblech@ya.ru", "Metalcorehero88", "debug тест");
}

void TwitterPopup::hide(EventGUIRef& event)
{
	disconnectListeners();
	Popup::hide(event);
}

void TwitterPopup::disconnectListeners()
{
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEND_TOUCH);

	twitter->postingComplete.disconnect_all_slots();
	twitter->postingError.disconnect_all_slots();
}

void TwitterPopup::draw()
{	
	Popup::draw();	
	gl::color(ColorA(1, 1, 1, alphaAnim));
	gl::draw(socialTexture, socialTexturePosition);
	gl::color(Color::white());
}