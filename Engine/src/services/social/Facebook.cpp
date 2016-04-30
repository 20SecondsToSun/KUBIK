#include "services/social/Facebook.h"

using namespace std;
using namespace kubik;
using namespace kubik::config;
using namespace ci;
using namespace ci::app;
using namespace mndl::curl;
using namespace Awesomium;

////////////////////////////////////////////////////////////////////////////
//
//				FACEBOOK
//
////////////////////////////////////////////////////////////////////////////

Facebook::Facebook() 
	:SocShare(),
	facebookAlbumNameToPost("Sokolniki")	
{
	initWebBrowserSize = Vec2f(770, 510);
	defaultStatus = SocialSettings::FACEBOOK_STATUS_DEFAULT;
	authURL = SocialSettings::FACEBOOK_AUTH_URL;
	availableArea = Rectf(0.f, 0.0f, 1080.f, 715.f);
}

void Facebook::update()
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

void Facebook::signInUpdate()
{
	auto anchorString = chrome().convertToString(mWebViewPtr->url().anchor());
	auto pos = anchorString.find("access_token");

	if (pos == 0)
	{
		string delimiter = "&";
		string token = anchorString.substr(0, anchorString.find(delimiter));
		access_token = token.substr(13);

		facebookPostThread();
	}
	else
	{
		auto queryString = chrome().convertToString(mWebViewPtr->url().query());
	
		size_t pos_denied = queryString.find("error_reason=user_denied");
		if (queryString.size() == 0 || pos_denied < 1000)
		{
			status = USER_REJECT;
			mWebViewPtr->LoadURL(WebURL(WSLit("")));
			hideSignal();
		}
	}
}

void  Facebook::facebookPostThread()
{
	status = POSTING;
	postingStart();
	loadingThread = ThreadRef(new boost::thread(&Facebook::posting, this));
}

void Facebook::postText(const std::string& textStatus)
{	
	map<string, string> strings;
	strings.insert(pair<string, string>("message", Utils::cp1251_to_utf8(textStatus.c_str())));
	strings.insert(pair<string, string>(SocialSettings::FACEBOOK_ACCESS_TOKEN, access_token));

	string fbRequest = Curl::post(SocialSettings::FACEBOOK_FEED_URL, strings);
	if (fbRequest != "")
	{
		try
		{
			JsonTree jTree = JsonTree(fbRequest);
			if (jTree.hasChild("id"))
			{
				console() << " POST FB OK!!!!!!!" << std::endl;
				status = POST_READY;
				return;
			}
		}
		catch (...)
		{

		}
	}

	status = POST_ERROR;
}

void Facebook::postPhoto(const std::string& textStatus, const std::vector<std::string>& filesPath)
{
	auto fbRequest = Curl::get(SocialSettings::FACEBOOK_ALBUMS_URL + "/?access_token=" + access_token);
	facebookAlbumId = SocialSettings::FACEBOOK_NULL_ALBUM_ID;
	
	try
	{
		JsonTree jTree = JsonTree(fbRequest);
		if (jTree.hasChild("data"))
		{
			JsonTree data = jTree.getChild("data");
			for (JsonTree::ConstIter item = data.begin(); item != data.end(); ++item)
			{
				string _name = item->getChild("name").getValue<string>();
				string _nameCyr = Utils::Utf8_to_cp1251(_name.c_str());
				if (_nameCyr == facebookAlbumNameToPost)
				{
					facebookAlbumId = item->getChild("id").getValue<string>();					
				}
			}
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

	//if (facebookAlbumId == SocialSettings::FACEBOOK_NULL_ALBUM_ID)
	{
		map<string, string> strings;
		strings.insert(pair<string, string>(SocialSettings::FACEBOOK_ACCESS_TOKEN, access_token));
		strings.insert(pair<string, string>("name", Utils::cp1251_to_utf8(facebookAlbumNameToPost.c_str())));
		string fbRequest = Curl::post(SocialSettings::FACEBOOK_ALBUMS_URL, strings);

		try
		{
			JsonTree jTree = JsonTree(fbRequest);
			console() << "fbRequest------------------------------>" << fbRequest << endl;
			if (jTree.hasChild("id"))
			{
				facebookAlbumId = jTree.getChild("id").getValue();

				if (facebookAlbumId != "")
				{
					console() << "postPhotosToFbAlbum------------------------------>"<< endl;
					postPhotosToFbAlbum(textStatus, filesPath);
					return;
				}
			}

			status = POST_ERROR;
			return;
		}
		catch (...)
		{
			status = POST_ERROR;
			return;
		}
	}
	/*else
	{
		status = POST_ERROR;
		return;
	}*/
}

void Facebook::postPhotosToFbAlbum(const std::string& textStatus, const std::vector<std::string>& filesPath)
{
	int success_upload = 0;
	string post_id{ "" };

	for (size_t i = 0, ilen = filesPath.size(); i < ilen; i++)
	{
		string fbRequest = Curl::postUploadFB(SocialSettings::FACEBOOK_BASE_URL + facebookAlbumId + "/photos/",
			access_token, filesPath[i],
			Utils::cp1251_to_utf8(textStatus.c_str()));
		try
		{
			JsonTree jTree = JsonTree(fbRequest);			
			
			if (jTree.hasChild("post_id"))
			{
				post_id = jTree.getChild("post_id").getValue();
				if (post_id != "")
				{
					success_upload++;
				}
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
	}

	if (success_upload > 0)
	{	
		auto pos = post_id.find("_");
		auto id1 = post_id.substr(0, pos);
		auto id2 = post_id.substr(pos + 1, post_id.size());
		auto postID = "http://www.facebook.com/" + id1 + "/posts/" + id2;
		linkToPost = postID;

		logger().log(postID);
		logger().log("-------------------");
		logger().log(post_id);

		status = POST_READY;
		return;
	}	

	status = POST_ERROR;
}

void Facebook::setPhotoAlbumName(const string &name)
{
	facebookAlbumNameToPost = name;
}

void Facebook::logOut()
{
	std::map<string, string> strings;
	strings.insert(pair<string, string>(SocialSettings::FACEBOOK_ACCESS_TOKEN, access_token));
	string fbRequest = Curl::deleteRequest(SocialSettings::FACEBOOK_APP_LOGOUT_URL, strings);	
}