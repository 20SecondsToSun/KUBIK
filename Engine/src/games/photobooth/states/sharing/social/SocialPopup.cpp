#include "states/sharing/social/SocialPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

SocialPopup::SocialPopup(PhotoboothSettingsRef settings)
	: Popup(settings),
	postingBg(settings->getTexture("popupNetralBg")),
	errorBg(settings->getTexture("popupErrorBg")),
	successMessage(settings->getTexture("successMessage")),
	errorMessage(settings->getTexture("errorMessage")),
	mainPreloader(settingsFactory().getMainPreloader()),
	miniPreloader(settingsFactory().getMiniPreloader())
{


}

void SocialPopup::show()
{
	social->clear_token();
	social->initChromium();

	hideSignalCon = social->hideSignal.connect(bind(&SocialPopup::close, this));
	postingStartSignalCon    = social->postingStart.connect(bind(&SocialPopup::postingStartHandler, this));
	postingCompleteSignalCon = social->postingComplete.connect(bind(&SocialPopup::postingCompleteHandler, this));
	postingErrorSignalCon	 = social->postingError.connect(bind(&SocialPopup::postingErrorHandler, this));

	Popup::show();

	popupDrawFunc = &SocialPopup::signInDraw;
}

void SocialPopup::postingStartHandler()
{
	Popup::disconnectCloseBtn();
	popupDrawFunc = &SocialPopup::postingDraw;
}

void SocialPopup::postingCompleteHandler()
{
	popupDrawFunc = &SocialPopup::successDraw;
	Popup::connectAllFieldAsButton();
}

void SocialPopup::postingErrorHandler()
{
	popupDrawFunc = &SocialPopup::errorDraw;
	Popup::connectAllFieldAsButton();
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
	social->update();
	(this->*popupDrawFunc)();
}

void SocialPopup::signInDraw()
{
	Popup::drawBackgrounds();

	gl::pushMatrices();
	gl::translate(0.0f, 0.5f*(715.0f - miniPreloader->getHeight()));
	miniPreloader->draw();
	gl::popMatrices();
	
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	social->draw();

	Popup::draw();
}

void SocialPopup::successDraw()
{	
	gl::draw(postingBg);
	drawTextMessage(successMessage);
}

void SocialPopup::postingDraw()
{	
	gl::draw(postingBg);

	gl::pushMatrices();
	gl::translate(0.0f, 0.5f*(1920.0f - mainPreloader->getHeight()));
	mainPreloader->draw();
	gl::popMatrices();
}

void SocialPopup::errorDraw()
{	
	gl::draw(errorBg);
	drawTextMessage(errorMessage);
}

void SocialPopup::drawTextMessage(const ci::gl::Texture& text)
{
	gl::draw(text, Vec2f((1080.0f - text.getWidth())*0.5f, 748.0f - 0.5f*(text.getHeight())));
}