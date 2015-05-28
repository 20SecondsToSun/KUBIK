#pragma once

#include "Chromium.h"
#include "VirtualKeyboard.h"
#include "Curl.h"

using namespace Awesomium;

namespace kubik
{
	typedef std::shared_ptr<class SocShare> SocShareRef;

	class SocShare
	{
	public:
		enum ActivityID
		{
			PHOTO_STATUS,
			TEXT_STATUS
		} 
		activity;

		enum ServerStatus
		{
			IDLE,
			WAITING_FOR_NETWORK,
			USER_REJECT,
			POSTING,
			POST_READY,
			POST_ERROR,
			GET_AUTH_URL
		}
		status;

		enum PostingStatus
		{
			WallPostVkontakteServerError,
			VkontaktePostOk
		}
		postingStatus;

		SocShare();

		virtual void logOut() = 0;

		void clear_token();

		void postStatus(const std::string& textStatus = "");
		void postPhoto(const std::string& path, const std::string& textStatus = "");
		void postPhoto(const std::vector<std::string>& path, const std::string& textStatus);
		void setLoginPassword(const std::string& login, const std::string& password);

		std::string	access_token;
		std::string	getResponse();
		std::string	response;
		std::string	getLastError();

		boost::signals2::signal<void(void)>	serverHandler, hideSignal, postingComplete, postingStart, postingError;

		virtual void initChromium();
		virtual void update();
		virtual void draw();
		void handleKeyDown();

		void connectTouchDown();
		void disconnectTouchDown();
		ServerStatus getStatus();

	protected:		
		std::string authURL;

		std::string	login, password, lastError, defaultStatus;
		ci::Rectf availableArea;
		bool connected;

		connection loadingSignal;
		ThreadRef loadingThread;

		WebCore*		 mWebCorePtr;
		WebView*		 mWebViewPtr;
		ci::gl::Texture  mWebTexture;
		ci::Vec2f		 popupPosition;
		ci::Vec2i		 initWebBrowserSize;

		virtual std::string	 getDefaultStatus() const;
		virtual void updatePopupPosition();

		virtual int getBrowserWidth() const;
		virtual int getBrowserHeight() const;
		virtual std::string getAuthUrl();
		virtual std::vector<std::string> getUploadPhotoPathVec() const;
		virtual void postText(const std::string& textStatus){};
		virtual void postPhoto(const std::string& textStatus, const std::vector<std::string>& filesPath){};
		virtual void posting();
		virtual std::string getPostingStatus() const{ return ""; };		

		void resizeWebBrowser(int width, int height);

	private:
		ci::signals::connection	keyDownCon, mouseDownCon, mouseUpCon;
		void mouseDown(MouseEvent &event);
		void mouseUp(MouseEvent &event);
		void keyDown(KeyEvent event);		
	};
}