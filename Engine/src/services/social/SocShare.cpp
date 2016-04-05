#include "services/social/SocShare.h"

using namespace std;
using namespace kubik;
using namespace ci;
using namespace ci::app;
using namespace mndl::curl;

void SocShare::posting()
{
	switch (activity)
	{
	case SocShare::PHOTO_STATUS:
		postPhoto(getPostingStatus(), getUploadPhotoPathVec());
		break;

	case SocShare::TEXT_STATUS:
		postText(getPostingStatus());
		break;
	}

	logOut();
}

void SocShare::clear_token()
{
	access_token = "";
}

std::string SocShare::getResponse()
{
	return response;
}

std::string SocShare::getLastError()
{
	return lastError;
}

void SocShare::initChromium()
{
	chrome().clearCookies();

	mWebCorePtr = chrome().getWebCorePtr();
	mWebViewPtr = chrome().getWebViewPtr();	

	if (mWebViewPtr)
	{
		mWebViewPtr->Resize(getBrowserWidth(), getBrowserHeight());
		mWebViewPtr->LoadURL(WebURL(WSLit(getAuthUrl().c_str())));
		mWebViewPtr->Focus();
	}

	status = WAITING_FOR_NETWORK;
}

void SocShare::update()
{	
	if (mWebCorePtr)
	{
		mWebCorePtr->Update();
	}

	if (mWebViewPtr && ph::awesomium::isDirty(mWebViewPtr))
	{
		try
		{
			gl::Texture::Format fmt;
			fmt.setMagFilter(GL_NEAREST);
			mWebTexture = ph::awesomium::toTexture(mWebViewPtr, fmt);
		}
		catch (const std::exception &e) 
		{
			console() << e.what() << std::endl;
		}

		char title[1024];
		mWebViewPtr->title().ToUTF8(title, 1024);		
	}

	updatePopupPosition();
}

void SocShare::updatePopupPosition()
{
	popupPosition = Vec2f(0.5f * (getWindowWidth() - getBrowserWidth()), 0.5f * (715.0f - getBrowserHeight()));
}

void SocShare::draw()
{
	if (mWebTexture && !mWebViewPtr->IsLoading() && status == WAITING_FOR_NETWORK)
	{
		gl::pushMatrices();
		gl::translate(popupPosition);
		gl::draw(mWebTexture);
		gl::popMatrices();
	}
}

void SocShare::handleKeyDown()
{
	if (touchKeyboard().isBackCode())
	{
		if (mWebViewPtr->focused_element_type() != 3 &&
			mWebViewPtr->focused_element_type() != 4) return;

		KeyEvent key = VirtualKeyboard::imitate_BACKSPACE_KEY_EVENT();
		ph::awesomium::handleKeyDown(mWebViewPtr, key);
	}
	else
	{
		char chr = touchKeyboard().getLastCode()[0];
		KeyEvent key(getWindow(), 1, chr, chr, 1, chr);
		ph::awesomium::handleKeyDown(mWebViewPtr, key, true, chr);
	}
}

void SocShare::connectTouchDown()
{
	if (connected) return;

	mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&SocShare::mouseDown, this, std::placeholders::_1));
	mouseUpCon   = getWindow()->getSignalMouseUp().connect(std::bind(&SocShare::mouseUp,	 this, std::placeholders::_1));
	keyDownCon   = getWindow()->getSignalKeyDown().connect(std::bind(&SocShare::keyDown,	 this, std::placeholders::_1));

	touchKeyboard().connectEventHandler(&SocShare::handleKeyDown, this, VirtualKeyboard::KEY_TOUCH);
	connected = true;
}

void SocShare::mouseDown(MouseEvent &event)
{
	if (availableArea.contains(event.getPos()))
	{
		MouseEvent mEvent = touchKeyboard().inititateMouseEvent(event.getPos() - popupPosition);
		ph::awesomium::handleMouseDown(mWebViewPtr, mEvent);
	}
}

void SocShare::mouseUp(MouseEvent &event)
{
	if (availableArea.contains(event.getPos()))
	{
		MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(event.getPos() - popupPosition);
		ph::awesomium::handleMouseUp(mWebViewPtr, mEvent);		
	}
}

void SocShare::keyDown(KeyEvent event)
{
	ph::awesomium::handleKeyDown(mWebViewPtr, event);
}

void SocShare::disconnectTouchDown()
{
	if (connected)
	{
		mouseDownCon.disconnect();
		mouseUpCon.disconnect();
		keyDownCon.disconnect();
		touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
		connected = false;
	}	
}

SocShare::ServerStatus SocShare::getStatus()
{
	return status;
}

std::string SocShare::getDefaultStatus() const
{
	return defaultStatus;
}

void SocShare::setPostingStatus(const std::string& status)
{
	defaultStatus = status;
}

int SocShare::getBrowserWidth() const
{
	return initWebBrowserSize.x;
}

int SocShare::getBrowserHeight() const
{
	return initWebBrowserSize.y;
}

std::string SocShare::getAuthUrl()
{
	return authURL;
}

std::vector<std::string> SocShare::getUploadPhotoPathVec() const
{
	return uploadPhotoPathVec;
}

void SocShare::setUploadPhotoPathVec(const std::vector<std::string>& vec)
{
	uploadPhotoPathVec = vec;
}

void SocShare::resizeWebBrowser(int width, int height)
{
	initWebBrowserSize = Vec2i(width, height);
	mWebViewPtr->Resize(width, height);	
}
