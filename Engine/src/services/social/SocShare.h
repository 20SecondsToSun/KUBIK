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
		enum activityID
		{
			PHOTO_STATUS,
			TEXT_STATUS
		};

		enum serverStatus
		{
			WAITING_FOR_NETWORK,
			USER_REJECT,
			POSTING,
			POST_READY,
			POST_ERROR
		}
		status;

		enum postingStatus
		{
			WallPostVkontakteServerError,
			VkontaktePostOk
		}
		postingStatus;

		SocShare();

		//virtual void post() = 0;
		virtual const char *getAuthUrl() = 0;
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

		void initChromium();
		virtual void update();
		void draw();
		void handleKeyDown();

		void connectTouchDown();
		void disconnectTouchDown();
		serverStatus getStatus();

	protected:		
		std::vector<std::string> photosVector;
		std::string	login, password, textStatus, lastError;									
		ci::Rectf availableArea;
		bool connected;

		connection loadingSignal;
		ThreadRef loadingThread;

		WebCore*		 mWebCorePtr;
		WebView*		 mWebViewPtr;
		ci::gl::Texture  mWebTexture;
		ci::Vec2f		 popupPosition;

		virtual std::string	 getDefaultStatus() = 0;
		virtual void updatePopupPosition();

	private:
		ci::signals::connection	keyDownCon, mouseDownCon, mouseUpCon;
		void mouseDown(MouseEvent &event);
		void mouseUp(MouseEvent &event);
		void keyDown(KeyEvent event);
	};
}