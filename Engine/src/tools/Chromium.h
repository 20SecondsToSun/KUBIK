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
				WebPreferences prefs;
				prefs.enable_plugins = false;
				prefs.enable_smooth_scrolling = true;
				prefs.user_stylesheet = WSLit("::-webkit-scrollbar { visibility: hidden; }");
				
				mWebCorePtr = WebCore::Initialize(WebConfig());
				session = mWebCorePtr->CreateWebSession(Awesomium::WSLit("soc"), prefs);
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

		std::string convertToString(const WebString& webString)
		{
			const int size = 10024;

			char buffer[size];			
			webString.ToUTF8(buffer, size);
			std::string queryString(buffer);

			return  queryString;
		}	

		std::string callJS(const std::string& funcName)// example : "doResize();"
		{
			if (mWebViewPtr)
			{
				string param = "window";
				JSValue js = mWebViewPtr->ExecuteJavascriptWithResult(WSLit(param.c_str()), Awesomium::WSLit(""));

				if (!js.IsUndefined())
				{
					JSObject jO = js.ToObject();
					JSArray arr;
					jO.InvokeAsync(Awesomium::WSLit(funcName.c_str()), arr);
				}
			}		
		}
	};

	inline Chromium& chrome() { return Chromium::getInstance(); };
}