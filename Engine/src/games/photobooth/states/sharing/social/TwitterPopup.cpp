#include "states/sharing/social/TwitterPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

TwitterPopup::TwitterPopup(PhotoboothSettingsRef settings)
	:SocialPopup(settings)
{
	createSocialContext();
}

void TwitterPopup::createSocialContext()
{
	social = Social::createTwitter();
}
//TwitterPopup::TwitterPopup(PhotoboothSettingsRef settings)
//	:Popup(settings)
//{
//	twitter = TwitterRef(new Twitter());
//	socialTexture = settings->getTexture("twitterPopupTexture");
//	socialTexturePosition = Vec2f(.5f * (1080.0f - socialTexture.getWidth()), 393.0f - socialTexture.getHeight() * 0.5f);
//
//	errorBg = settings->getTexture("error_bg");
//	okPopup = settings->getTexture("okPopup");
//
//	loginField = TextFieldRef(new TextField(Rectf(407, 318, 407 + 285, 345)));
//	passField = TextFieldRef(new TextField(Rectf(407, 363, 407 + 285, 390)));
//
//	loginField->setTextColor(Color(0.0f, 0.0f, 0.0f));
//	passField->setTextColor(Color(0.0f, 0.0f, 0.0f));
//
//	loginField->setFont(settings->getFont("arial13"));
//	passField->setFont(settings->getFont("arial13"));
//
//	loginField->setColor(Color::hex(0xf9f9fa));
//	passField->setColor(Color::hex(0xf9f9fa));	
//
//	enterBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(400, 420, 500, 460)));
//	enterBtn->setAlpha(0.0f);
//}
//
//void TwitterPopup::show()
//{
//	loginField->clear();
//	passField->clear();
//
//	loginField->setTouched(false);
//	passField->setTouched(false);
//
//	activeField = nullptr;
//	setState(SOCIAL_DRAWING);
//	Popup::show();
//}
//
//void TwitterPopup::initVirtualKeyboard()
//{
//	touchKeyboard().setInputFieldVisible(false);
//	Popup::initVirtualKeyboard();
//}
//
//void TwitterPopup::showAnimComplete()
//{
//	connectKeyboard();	
//
//	loginField->connectEventHandler(&TwitterPopup::loginTouchHandler, this);
//	passField->connectEventHandler(&TwitterPopup::passTouchHandler, this);
//	enterBtn->connectEventHandler(&TwitterPopup::sendButtonHandler, this);
//
//	connect_once(twitter->postingComplete, bind(&TwitterPopup::postingCompleteHandler, this));
//	connect_once(twitter->postingError, bind(&TwitterPopup::postingErrorHandler, this));
//	connect_once(twitter->gettingAuthURLComplete, bind(&TwitterPopup::gettingAuthURLCompleteHandler, this));
//
//	Popup::showAnimComplete();	
//}
//
//void TwitterPopup::connectKeyboard()
//{
//	touchKeyboard().connectEventHandler(&TwitterPopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
//	touchKeyboard().connectEventHandler(&TwitterPopup::sendHandler, this, VirtualKeyboard::SEND_TOUCH);
//}
//
//void TwitterPopup::disconnectKeyboard()
//{
//	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
//	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEND_TOUCH);
//}
//
//void TwitterPopup::postingCompleteHandler()
//{
//	setState(SOCIAL_OK);
//}
//
//void TwitterPopup::postingErrorHandler()
//{
//	setState(SOCIAL_ERROR);
//}
//
//void TwitterPopup::gettingAuthURLCompleteHandler()
//{
//	console() << "twitter->getAuthURL()!!!!!!!!!!" << twitter->getAuthURL()<< endl;
//	mWebCorePtr = chrome().getWebCorePtr();
//	mWebViewPtr = chrome().getWebViewPtr();
//	chrome().clearCookies();
//
//	if (mWebViewPtr)
//	{
//		mWebViewPtr->Resize(675, 440);
//		mWebViewPtr->LoadURL(WebURL(WSLit(twitter->getAuthURL().c_str())));
//		mWebViewPtr->Focus();
//	}
//
//	setState(AUTHORIZATION);
//}
//
//void TwitterPopup::keyTouchHandler()
//{
//	//console() << "::::::touch " << touchKeyboard().getLastCode() << endl;	
//	if (activeField)
//		activeField->addLetter(touchKeyboard().getLastCode());
//}
//
//void TwitterPopup::sendHandler()
//{
//	console() << "send!!!!!!!!!!" << endl;
//
//	if (loginField->empty())
//		loginField->showThatEmpty();
//	else if (passField->empty())
//		passField->showThatEmpty();
//	else
//	{
//		setState(SOCIAL_POSTING);
//		twitter->authorizePost("yurikblech@ya.ru", "Metalcorehero88", "debug тест");
//	}
//}
//
//void TwitterPopup::hide(EventGUIRef& event)
//{
//	disconnectListeners();
//	Popup::hide(event);
//}
//
//void TwitterPopup::disconnectListeners()
//{
//	disconnectKeyboard();
//
//	twitter->postingComplete.disconnect_all_slots();
//	twitter->postingError.disconnect_all_slots();
//
//	disconnectEventHandler();
//
//	loginField->disconnectEventHandler();
//	passField->disconnectEventHandler();
//	enterBtn->disconnectEventHandler();	
//}
//
//void TwitterPopup::draw()
//{	
//	switch (state)
//	{
//	case SOCIAL_POSTING:
//		drawBackgrounds();
//		break;
//
//	case SOCIAL_ERROR:
//		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
//		gl::draw(errorBg);
//		break;
//
//	case SOCIAL_OK:
//		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
//		gl::draw(okPopup);
//		break;
//
//	case SOCIAL_DRAWING:
//		Popup::draw();
//		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
//		gl::draw(socialTexture, socialTexturePosition);
//	
//		loginField->setAlpha(alphaAnim);
//		passField->setAlpha(alphaAnim);
//		//enterBtn->setAlpha(alphaAnim);
//
//		loginField->draw();
//		passField->draw();		
//		enterBtn->draw();
//		break;
//
//	case AUTHORIZATION:
//		chromeUpdate();
//		chromeDraw();
//		break;
//	}
//}
//
//
//void TwitterPopup::chromeUpdate()
//{
//	mWebCorePtr->Update();
//
//	if (mWebViewPtr && ph::awesomium::isDirty(mWebViewPtr))
//	{
//		try
//		{
//			gl::Texture::Format fmt;
//			fmt.setMagFilter(GL_NEAREST);
//			mWebTexture = ph::awesomium::toTexture(mWebViewPtr, fmt);
//		}
//		catch (const std::exception &e) {
//			console() << e.what() << std::endl;
//		}
//
//		char title[1024];
//		mWebViewPtr->title().ToUTF8(title, 1024);
//	}	
//}
//
//void TwitterPopup::chromeDraw()
//{
//	if (mWebTexture  && !mWebViewPtr->IsLoading())
//	{
//		gl::pushMatrices();
//		gl::draw(mWebTexture);
//		gl::popMatrices();
//	}
//}
//
//void TwitterPopup::setState(const SocialPopupState& value)
//{
//	if (state == value)
//		return;		
//
//	switch (state)
//	{
//	case SOCIAL_POSTING:
//		disconnectKeyboard();
//		break;	
//	}
//
//	state = value;
//
//	switch (state)
//	{
//	case SOCIAL_OK:
//		connectEventHandler(&TwitterPopup::closePopupHandler, this);
//	break;
//
//	case SOCIAL_ERROR:
//		connectEventHandler(&TwitterPopup::tryAgainHandler, this);
//		break;
//	}
//}
//
//void TwitterPopup::closePopupHandler(EventGUIRef& _event)
//{	
//	disconnectListeners();
//	Popup::hide(event);
//}
//
//void TwitterPopup::tryAgainHandler(EventGUIRef& _event)
//{
//	disconnectEventHandler();
////	connectEventHandler(&TwitterPopup::touchHandler, this);
//	setState(SOCIAL_DRAWING);
//}
//
//void TwitterPopup::sendButtonHandler(EventGUIRef& _event)
//{
//	sendHandler();
//}
//
//void TwitterPopup::loginTouchHandler(EventGUIRef& _event)
//{
//	activeField = loginField;
//	loginField->setTouched(true);
//	passField->setTouched(false);
//}
//
//void TwitterPopup::passTouchHandler(EventGUIRef& _event)
//{
//	activeField = passField;
//	loginField->setTouched(false);
//	passField->setTouched(true);
//}