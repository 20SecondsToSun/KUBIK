#include "services/social/Vkontakte.h"

using namespace std;
using namespace kubik;
using namespace kubik::config;
using namespace ci;
using namespace ci::app;
using namespace mndl::curl;

Vkontakte::Vkontakte() :SocShare()
{
	availableArea = Rectf(228.f, 295.f, 862.f, 536.f);
}

void Vkontakte::updatePopupPosition()
{
	if (mWebTexture)
		popupPosition = Vec2f(0.5f * (getWindowWidth() - mWebTexture.getWidth()), 166.0f);
	else
		popupPosition = Vec2f::zero();
}

void Vkontakte::update()
{
	SocShare::update();

	if (status != WAITING_FOR_NETWORK)
		return;

	char anchr[1024];
	mWebViewPtr->url().anchor().ToUTF8(anchr, 1024);
	string anchString(anchr);
	size_t pos = anchString.find("access");

	if (pos == 0 && status == WAITING_FOR_NETWORK)
	{
		string delimiter = "&";
		string token = anchString.substr(0, anchString.find(delimiter));
		access_token = token.substr(13);
		vkontaktePostThread();
	}
	else
	{
		size_t pos_denied = anchString.find("User denied your request");
		if (pos_denied < 1000)
		{
			status = USER_REJECT;
			hideSignal();
		}
	}

	/*if (!isTryFocusInLoginTextField && mWebTexture && mWebViewPtr->IsLoading() == false)
	{
	MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(Vec2f(932.0f, 246.0f));
	isTryFocusInLoginTextField = true;
	ph::awesomium::handleMouseDown(mWebViewPtr, mEvent);
	ph::awesomium::handleMouseUp(mWebViewPtr, mEvent);
	}
	*/
}

void  Vkontakte::vkontaktePostThread()
{
	status = POSTING;
	postingStart();

	loadingSignal = App::get()->getSignalUpdate().connect(bind(&Vkontakte::waitLoadingComplete, this));
	loadingThread = ThreadRef(new boost::thread(&Vkontakte::posting, this));
}

void  Vkontakte::waitLoadingComplete()
{
	if (status != POSTING)
	{
		loadingThread->join();
		loadingSignal.disconnect();
		postingComplete();
	}
}

void  Vkontakte::posting()
{
	postTextVK();
	logOut();
}

void Vkontakte::postTextVK()
{
	if (textStatus.empty())
		textStatus = SocialSettings::VK_STATUS_DEFAULT;

	std::map<string, string> strings;
	strings.insert(pair<string, string>("message", Utils::cp1251_to_utf8(textStatus.c_str())));
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	string vkRequest = Curl::post(SocialSettings::VK_WALL_POST_URL, strings);

	if (vkRequest != "")
	{
		try
		{
			JsonTree jTree = JsonTree(vkRequest);
			if (jTree.hasChild("response"))
			{
				if (jTree.getChild("response").hasChild("post_id"))
				{
					status = POST_READY;
					return;
				}
					
			}
		}
		catch (...)
		{

		}
	}

	status = POST_ERROR;
}

void Vkontakte::postPhotoVK()
{
	std::map<string, string> strings;
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	string vkRequest = Curl::post(SocialSettings::VK_WALL_UPLOAD_SERVER_URL, strings);
	string upload_url = "";
	try
	{
		JsonTree jTree = JsonTree(vkRequest);
		if (jTree.hasChild("response"))
		{
			upload_url = jTree.getChild("response").getChild("upload_url").getValue();
		}
		else
		{
			status = POST_ERROR;
			return;
		}
	}
	catch (...)
	{
		status = POST_ERROR;
		return;
	}

	string attacments = "";

	for (size_t i = 0, ilen = photosVector.size(); i < ilen; i++)
	{
		try
		{
			string photo_id = vkontaktePostLoadPhotoPath(upload_url, photosVector[i]);
			if (photo_id != "")
			{
				attacments += photo_id;
				if (i != 3) attacments += ",";
			}
		}
		catch (...)
		{
			status = POST_ERROR;
			return;
		}	
	}

	console() << "ATTACHMENTS:::  " << attacments << std::endl;

	strings.clear();
	strings.insert(pair<string, string>("attachments", attacments));
	strings.insert(pair<string, string>("message", Utils::cp1251_to_utf8(textStatus.c_str())));
	strings.insert(pair<string, string>("access_token", access_token));

	vkRequest = Curl::post(SocialSettings::VK_WALL_POST_URL, strings);

	if (vkRequest != "")
	{
		try
		{
			JsonTree jTree = JsonTree(vkRequest);
			if (jTree.hasChild("response"))
			{
				if (jTree.getChild("response").hasChild("post_id"))
				{
					status = POST_ERROR;
					return;
				}					
			}
		}
		catch (...)
		{

		}
	}

	status = POST_ERROR;
}

string Vkontakte::vkontaktePostLoadPhotoPath(const string& upload_url, const string& path)
{
	std::map<string, string> strings;
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	string vkRequest = Curl::postUpload(upload_url, strings, path);

	JsonTree jTree = JsonTree(vkRequest);

	strings.clear();
	strings.insert(pair<string, string>("server", jTree.getChild("server").getValue()));
	strings.insert(pair<string, string>("photo", jTree.getChild("photo").getValue()));
	strings.insert(pair<string, string>("hash", jTree.getChild("hash").getValue()));
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	vkRequest = Curl::post(SocialSettings::VK_SAVE_WALL_PHOTO_URL, strings);
	string photo_id = "";

	jTree = JsonTree(vkRequest);

	if (jTree.hasChild("response[0]"))	
		photo_id = jTree.getChild("response[0]").getChild("id").getValue();	

	return photo_id;
}

const char * Vkontakte::getAuthUrl()
{
	return SocialSettings::VK_AUTH_URL.c_str();
}

void Vkontakte::logOut()
{
	string  logout = SocialSettings::VK_LOGOUT_URL + access_token;
	const char * url = logout.c_str();
	string vkRequest = Curl::get(url);
}

string Vkontakte::getDefaultStatus()
{
	return SocialSettings::VK_STATUS_DEFAULT;
}