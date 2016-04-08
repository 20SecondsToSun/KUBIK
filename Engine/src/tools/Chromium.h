#pragma once

#include "TextItem.h"
#include "CinderAwesomium.h"

namespace kubik
{
	class Chromium
	{
	public:
		static Chromium& getInstance()
		{
			static Chromium chrome;
			return chrome;
		};

		Awesomium::WebCore*		 mWebCorePtr;
		Awesomium::WebView*		 mWebViewPtr;
		Awesomium::WebSession*		 session;

		void init();

		Awesomium::WebCore* getWebCorePtr();
		Awesomium::WebView* getWebViewPtr();
		void clearCookies();

		std::string convertToString(const Awesomium::WebString& webString);
		void callJS(const std::string& funcName);
	};

	inline Chromium& chrome() { return Chromium::getInstance(); };
}