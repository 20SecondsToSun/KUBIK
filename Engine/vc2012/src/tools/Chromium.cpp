#include "Chromium.h"

using namespace ci;
using namespace kubik;
using namespace std;
using namespace Awesomium;

void Chromium::init()
{
	if (!Awesomium::WebCore::instance())
	{
		Awesomium::WebPreferences prefs;
		prefs.enable_plugins = false;
		prefs.enable_smooth_scrolling = true;
		prefs.user_stylesheet = WSLit("::-webkit-scrollbar { visibility: hidden; }");

		mWebCorePtr = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
		session = mWebCorePtr->CreateWebSession(Awesomium::WSLit("soc"), prefs);
		mWebViewPtr = mWebCorePtr->CreateWebView(100, 100, session);
	}
	else
	{
		mWebCorePtr = Awesomium::WebCore::instance();
	}
}

Awesomium::WebCore* Chromium::getWebCorePtr()
{
	return mWebCorePtr;
}

Awesomium::WebView* Chromium::getWebViewPtr()
{
	return mWebViewPtr;
}

void Chromium::clearCookies()
{
	session->ClearCookies();
}

std::string Chromium::convertToString(const Awesomium::WebString& webString)
{
	const int size = 10024;

	char buffer[size];
	webString.ToUTF8(buffer, size);
	std::string queryString(buffer);

	return queryString;
}

void Chromium::callJS(const std::string& funcName)// example : "doResize();"
{
	if (mWebViewPtr)
	{
		std::string param = "window";
		JSValue js = mWebViewPtr->ExecuteJavascriptWithResult(WSLit(param.c_str()), Awesomium::WSLit(""));

		if (!js.IsUndefined())
		{
			JSObject jO = js.ToObject();
			JSArray arr;
			jO.InvokeAsync(Awesomium::WSLit(funcName.c_str()), arr);
		}
	}
}
