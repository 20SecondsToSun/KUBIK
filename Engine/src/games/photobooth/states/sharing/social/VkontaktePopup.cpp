#include "states/sharing/social/VkontaktePopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

using namespace Awesomium;

WebCore*		 VkontaktePopup::mWebCorePtr;
WebView*		 VkontaktePopup::mWebViewPtr;
WebSession*		 VkontaktePopup::session;

VkontaktePopup::VkontaktePopup(PhotoboothSettingsRef settings)
	:Popup(settings)
{
	mWebCorePtr = WebCore::Initialize(WebConfig());
	session = mWebCorePtr->CreateWebSession(Awesomium::WSLit("soc"), WebPreferences());
	mWebViewPtr = mWebCorePtr->CreateWebView(getWindowWidth(), getWindowHeight(), session);
}

void VkontaktePopup::show()
{

	//social = new Vkontakte();
	/*social->clear_token();
	socialServerSignalCon = social->serverHandler.connect(
		boost::bind(&PopupBase::socialServerSignal, this)
		);

	postingWaitingText = vkontakteParams::POSTING_WAITING_TEXT;*/

	
	if (mWebViewPtr)
		mWebViewPtr->Resize(500, 500);

	mWebViewPtr->LoadURL(WebURL(WSLit("http://google.com")));// social->getAuthUrl())));
	mWebViewPtr->Focus();

	Popup::show();
}

void VkontaktePopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void VkontaktePopup::showAnimComplete()
{
	touchKeyboard().connectEventHandler(&VkontaktePopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
	Popup::showAnimComplete();	
}

void VkontaktePopup::keyTouchHandler()
{
	console() << "::::::touch " << touchKeyboard().getLastCode() << endl;
}

void VkontaktePopup::hide(EventGUIRef& event)
{
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
	Popup::hide(event);
}

void VkontaktePopup::draw()
{

	mWebCorePtr->Update();

	if (mWebViewPtr && ph::awesomium::isDirty(mWebViewPtr))
	{
		try
		{
			gl::Texture::Format fmt;
			fmt.setMagFilter(GL_NEAREST);
			mWebTexture = ph::awesomium::toTexture(mWebViewPtr, fmt);
		}
		catch (const std::exception &e) {
			console() << e.what() << std::endl;
		}

		char title[1024];
		mWebViewPtr->title().ToUTF8(title, 1024);
	}

	Popup::draw();

	if (mWebTexture  && !mWebViewPtr->IsLoading())// && socialServerStatus != USER_REJECT)
	{
		/*if (animBegin == false)
		{
			animBegin = true;
			timeline().apply(&popupAnimationPos, -600.0f, 0.0f, 0.4f, EaseOutQuart());
		}*/
		gl::pushMatrices();
		//gl::translate(bgPosition);
		//gl::translate(0.0f, 674.0f);		
		//gl::translate(0.5f*(getWindowWidth() - mWebTexture.getWidth()), popupAnimationPos);
		gl::color(Color::white());
		gl::draw(mWebTexture);
		gl::popMatrices();
	}
}