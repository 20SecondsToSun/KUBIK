#include "services/social/Vkontakte.h"

using namespace std;
using namespace kubik;
using namespace kubik::config;
using namespace ci;
using namespace ci::app;
using namespace mndl::curl;

Vkontakte::Vkontakte() :SocShare()
{
	initWebBrowserSize = Vec2f(1080, 715);////570);////377);
	defaultStatus = SocialSettings::VK_STATUS_DEFAULT;
	authURL = SocialSettings::VK_AUTH_URL;

	availableArea = Rectf(228.f, 0.f, 862.f, 715.f);
}

void Vkontakte::update()
{
	switch (status)
	{
	case WAITING_FOR_NETWORK:
		signInUpdate();
		break;

	case POST_READY:
		postingComplete();
		status = IDLE;
		break;

	case POST_ERROR:
		postingError();
		status = IDLE;
		break;
	}

	SocShare::update();	
}

void  Vkontakte::signInUpdate()
{	
	std::string anchorString = chrome().convertToString(mWebViewPtr->url().anchor());
	size_t pos = anchorString.find("access");

	if (pos == 0)
	{
		string delimiter = "&";
		string token = anchorString.substr(0, anchorString.find(delimiter));
		access_token = token.substr(13);
		vkontaktePostThread();
	}
	else
	{
		size_t pos_denied = anchorString.find("User denied your request");
		if (pos_denied < 1000)
		{
			status = USER_REJECT;
			hideSignal();
		}
	}
	//
	//string param = "document.getElementsByTagName('html')[0].innerHTML";
	//WebString html = mWebViewPtr->ExecuteJavascriptWithResult(WSLit(param.c_str()), Awesomium::WSLit("")).ToString();
	//	
	//std::string htmlString = chrome().convertToString(html);			
	////console() << "==============================================" << endl << endl;
	////console() << htmlString << endl;

	//size_t oauthPos	  = htmlString.find("oauth_error");
	//size_t captchaPos = htmlString.find("captcha_key");
	//size_t undefinedPos = htmlString.find("undefined");

	//if (undefinedPos != std::string::npos)
	//	return;

	//if (oauthPos != std::string::npos && captchaPos != std::string::npos)
	//{
	//	resizeWebBrowser(656, 570);		
	//}
	//else if(oauthPos != std::string::npos)
	//{			
	//	resizeWebBrowser(656, 435);
	//}
	//else if (captchaPos != std::string::npos)
	//{		
	//	resizeWebBrowser(656, 520);
	//}
	//else
	//{	
	//	resizeWebBrowser(656, 420);
	//}
	
}

void  Vkontakte::vkontaktePostThread()
{
	status = POSTING;
	postingStart();

	loadingThread = ThreadRef(new boost::thread(&Vkontakte::posting, this));
}

void Vkontakte::postText(const std::string& textStatus)
{
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

void Vkontakte::postPhoto(const std::string& textStatus, const std::vector<std::string>& filesPath)
{
	std::map<string, string> strings;
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	string vkRequest = Curl::post(SocialSettings::VK_WALL_UPLOAD_SERVER_URL, strings);
	string upload_url = "";
	linkToPost = "vk.com/wall";
	try
	{
		JsonTree jTree = JsonTree(vkRequest);
		if (jTree.hasChild("response"))
		{
			upload_url = jTree.getChild("response").getChild("upload_url").getValue();

			auto pos = upload_url.find("mid=");
			auto trimString = upload_url.substr(pos + 4, upload_url.size());
			auto pos2 = trimString.find("&");
			auto result = trimString.substr(0, pos2);
			linkToPost += result +"_";

			trimString.substr(pos, upload_url.size());
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

	string attacments{ "" };

	for (size_t i = 0, ilen = filesPath.size(); i < ilen; i++)
	{
		try
		{
			string photo_id = vkontaktePostLoadPhotoPath(upload_url, filesPath[i]);
			if (photo_id != "")
			{
				attacments += photo_id;
				if (i != 3)
				{
					attacments += ",";
				}
			}
		}
		catch (...)
		{
			status = POST_ERROR;
			return;
		}	
	}

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
					linkToPost += jTree.getChild("response").getChild("post_id").getValue();
					status = POST_READY;
					return;
				}					
			}
		}
		catch (...)
		{
			status = POST_ERROR;
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
	strings.insert(pair<string, string>("photo",  jTree.getChild("photo").getValue()));
	strings.insert(pair<string, string>("hash",	  jTree.getChild("hash").getValue()));
	strings.insert(pair<string, string>(SocialSettings::VK_ACCESS_TOKEN, access_token));

	vkRequest = Curl::post(SocialSettings::VK_SAVE_WALL_PHOTO_URL, strings);
	string photo_id = "";

	jTree = JsonTree(vkRequest);

	if (jTree.hasChild("response[0]"))
	{
		photo_id = jTree.getChild("response[0]").getChild("id").getValue();
	}

	return photo_id;
}

void Vkontakte::logOut()
{
	string  logout = SocialSettings::VK_LOGOUT_URL + access_token;
	const char * url = logout.c_str();
	string vkRequest = Curl::get(url);
}

std::string Vkontakte::getPostingStatus() const
{
	return getDefaultStatus();
}