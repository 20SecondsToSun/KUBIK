#include "services/social/Facebook.h"

using namespace std;
using namespace kubik;
using namespace kubik::config;
using namespace ci;
using namespace ci::app;
using namespace mndl::curl;

////////////////////////////////////////////////////////////////////////////
//
//				FACEBOOK
//
////////////////////////////////////////////////////////////////////////////

Facebook::Facebook()
	:SocShare()
{
	availableArea = (Rectf(256.f, 234.f, 897.f, 638.f));
}

void Facebook::updatePopupPosition()
{
	if (mWebTexture)
		popupPosition = Vec2f(0.5f * (getWindowWidth() - mWebTexture.getWidth()), 166.0f);
	else
		popupPosition = Vec2f::zero();
}

void Facebook::update()
{
	SocShare::update();

	if (status != WAITING_FOR_NETWORK)
		return;

	char anchr[1024];
	mWebViewPtr->url().anchor().ToUTF8(anchr, 1024);
	string anchString(anchr);
	size_t pos = anchString.find("access_token");

	if (pos == 0 && status == WAITING_FOR_NETWORK)
	{
		string delimiter = "&";
		string token = anchString.substr(0, anchString.find(delimiter));
		access_token = token.substr(13);

		facebookPostThread();		
	}
	else
	{
		char query[1024];
		mWebViewPtr->url().query().ToUTF8(query, 1024);
		string queryString(query);
		size_t pos_denied = queryString.find("error_reason=user_denied");
		if (queryString.size() == 0 || pos_denied < 1000)
		{
			status = USER_REJECT;
			hideSignal();
		}
	}
}

void  Facebook::facebookPostThread()
{
	status = POSTING;
	postingStart();

	loadingSignal = App::get()->getSignalUpdate().connect(bind(&Facebook::waitLoadingComplete, this));
	loadingThread = ThreadRef(new boost::thread(&Facebook::posting, this));
}

void  Facebook::waitLoadingComplete()
{
	if (status != POSTING)
	{
		loadingThread->join();
		loadingSignal.disconnect();
		postingComplete();
	}
}

void  Facebook::posting()
{
	postTextFB();
	logOut();
}

void Facebook::postTextFB()
{
	if (textStatus.empty())
		textStatus = SocialSettings::FACEBOOK_STATUS_DEFAULT;

	map<string, string> strings;
	strings.insert(pair<string, string>("message", Utils::cp1251_to_utf8(textStatus.c_str())));
	strings.insert(pair<string, string>(SocialSettings::FACEBOOK_ACCESS_TOKEN, access_token));

	string fbRequest = Curl::post(SocialSettings::FACEBOOK_FEED_URL, strings);
	console() << " fbRequest" << fbRequest << "   " << access_token<<std::endl;
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

string Facebook::getDefaultStatus()
{
	return  SocialSettings::FACEBOOK_STATUS_DEFAULT;
}

/*

string Facebook::postPhotoFB()
{
	string fbRequest = Curl::get(FACEBOOK_ALBUMS_URL + "/?access_token=" + access_token);
	facebookAlbumId = NULL_ALBUM_ID;

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
		else return FacebookAlbumAccessError;
	}
	catch (...)
	{
		return FacebookAlbumAccessError;
	}

	if (facebookAlbumId == NULL_ALBUM_ID)
	{
		map<string, string> strings;
		strings.insert(pair<string, string>(ACCESS_TOKEN, access_token));
		strings.insert(pair<string, string>("name", Utils::cp1251_to_utf8(facebookAlbumNameToPost.c_str())));
		string fbRequest = Curl::post(FACEBOOK_ALBUMS_URL, strings);

		try
		{
			JsonTree jTree = JsonTree(fbRequest);
			if (jTree.hasChild("id"))
			{
				facebookAlbumId = jTree.getChild("id").getValue();

				if (facebookAlbumId != "")
				{
					return postPhotosToFbAlbum();
				}
			}

			return FacebookAlbumAccessError;
		}
		catch (...)
		{
			return FacebookAlbumAccessError;
		}
	}
	else
	{
		return postPhotosToFbAlbum();
	}
}

string Facebook::postPhotosToFbAlbum()
{
	int success_upload = 0;

	for (size_t i = 0, ilen = photosVector.size(); i < ilen; i++)
	{
		string fbRequest = Curl::postUploadFB(FACEBOOK_BASE_URL + facebookAlbumId + "/photos/",
			access_token, photosVector[i],
			Utils::cp1251_to_utf8(textStatus.c_str()));
		try
		{
			JsonTree jTree = JsonTree(fbRequest);
			if (jTree.hasChild("post_id"))
			{
				string post_id = jTree.getChild("post_id").getValue();
				if (post_id != "") success_upload++;
			}
			else return FacebookPostPhotoError;
		}
		catch (...)
		{
			return FacebookPostPhotoError;
		}
	}

	if (success_upload > 0) return FacebookPostOk;

	return FacebookPostPhotoError;
}

void Facebook::setPhotoAlbumName(string &name)
{
	facebookAlbumNameToPost = name;
}
*/
std::string Facebook::getAuthUrl()
{
	return SocialSettings::FACEBOOK_AUTH_URL;
}

void Facebook::logOut()
{
	std::map<string, string> strings;
	strings.insert(pair<string, string>(SocialSettings::FACEBOOK_ACCESS_TOKEN, access_token));
	string fbRequest = Curl::deleteRequest(SocialSettings::FACEBOOK_APP_LOGOUT_URL, strings);

	/*if (fbRequest != "true")
	{
	lastError = FacebookLogOutError;
	}*/
}

/*
string  Facebook::getDefaultStatus()
{
	return STATUS_DEFAULT;
}*/