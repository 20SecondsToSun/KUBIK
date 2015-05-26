#include "states/sharing/social/SocialPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

SocialPopup::SocialPopup(PhotoboothSettingsRef settings) : Popup(settings)
{

}

void SocialPopup::show()
{
	social->clear_token();
	social->initChromium();

	hideSignalCon			 = social->hideSignal.connect(bind(&SocialPopup::close, this));
	postingStartSignalCon	 = social->postingStart.connect(bind(&SocialPopup::postingStartHandler, this));
	postingCompleteSignalCon = social->postingComplete.connect(bind(&SocialPopup::postingCompleteHandler, this));
	postingErrorSignalCon	 = social->postingError.connect(bind(&SocialPopup::postingCompleteHandler, this));

	Popup::show();
}

void SocialPopup::postingStartHandler()
{
	Popup::disconnectCloseBtn();
}

void SocialPopup::postingCompleteHandler()
{
	Popup::connectCloseBtn();
	close();
}

void SocialPopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void SocialPopup::showAnimComplete()
{
	social->connectTouchDown();
	Popup::showAnimComplete();
}

void SocialPopup::hide(EventGUIRef& event)
{
	close();
}

void SocialPopup::close()
{
	disconnectSignals();
	Popup::close();
}

void SocialPopup::kill()
{
	disconnectSignals();
	Popup::kill();
}

void SocialPopup::disconnectSignals()
{
	social->disconnectTouchDown();
	hideSignalCon.disconnect();
	postingStartSignalCon.disconnect();
	postingCompleteSignalCon.disconnect();
	postingErrorSignalCon.disconnect();
}

void SocialPopup::draw()
{
	Popup::draw();

	social->update();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	social->draw();
}