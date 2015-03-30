#pragma once

#include "TextItem.h"
#include "CinderAwesomium.h"

using namespace Awesomium;

namespace kubik
{
	class Chromium
	{
	public:

		static Chromium& getInstance() {
			static Chromium chrome;
			return chrome;
		};

		WebCore*		 mWebCorePtr;
		WebView*		 mWebViewPtr;
		WebSession*		 session;

		void init()
		{
			if (!WebCore::instance())
			{
				mWebCorePtr = WebCore::Initialize(WebConfig());
				session		= mWebCorePtr->CreateWebSession(Awesomium::WSLit("soc"), WebPreferences());
				mWebViewPtr = mWebCorePtr->CreateWebView(100, 100, session);
			}
			else
				mWebCorePtr = WebCore::instance();
		}

		WebCore* getWebCorePtr()
		{
			return mWebCorePtr;
		}

		WebView* getWebViewPtr()
		{
			return mWebViewPtr;
		}

		void clearCookies()
		{
			session->ClearCookies();
		}
	};

	inline Chromium& chrome() { return Chromium::getInstance(); };
}