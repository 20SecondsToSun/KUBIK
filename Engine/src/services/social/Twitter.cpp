#include "Twitter.h"

using namespace kubik::config;
using namespace kubik;
using namespace std;

Twitter::Twitter() : isAuthFlowComplete(false)
{
	availableArea = Rectf(0.0f, 0.0f, 897.0f, 538.0f);
};

void Twitter::post()
{
	if (isAuthFlowComplete && status != POSTING)
	{
		status = POSTING;
		postingStart();

		switch (activity)
		{
			case SocShare::PHOTO_STATUS:				
				loadingThread = ThreadRef(new boost::thread(&Twitter::postPhotoTweet, this, getPostingStatus(), getUploadPhotoPathVec()));
				break;

			case SocShare::TEXT_STATUS:
				loadingThread = ThreadRef(new boost::thread(&Twitter::postTextTweet, this, getPostingStatus()));
				break;
		}
	}
}

void Twitter::postTextTweet(const std::string& textStatus)
{
	if (isAuthFlowComplete)
	{
		std::string replyMsg = "";

		if (!twitterObj.statusUpdate(Utils::cp1251_to_utf8(textStatus.c_str())))
			twitterObj.getLastCurlError(replyMsg);

		twitterObj.getLastWebResponse(replyMsg);

		console() << "twitter callback:::  " << replyMsg << endl;

		status = POST_READY;
	}
}

void Twitter::postPhotoTweet(const std::string& textStatus, const std::vector<std::string>& filesPath)
{
	if (!isAuthFlowComplete)
		return;

	int max_media_per_upload;
	std::string replyMsg = "";

	if (twitterObj.getTwitterConfiguration())
	{
		twitterObj.getLastWebResponse(replyMsg);
		try
		{
			JsonTree jTree = JsonTree(replyMsg);
			max_media_per_upload = atoi(jTree.getChild("max_media_per_upload").getValue().c_str());
			console() << "max_media_per_upload:  " << max_media_per_upload << endl;
		}
		catch (...){};
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		console() << "twitterClient:: " << replyMsg.c_str() << std::endl;
		status = POST_ERROR;
		return;
	}

	vector<string> filelinks;

	for (int i = 0; i < max_media_per_upload; i++)
	{
		console() << "path ::  " << filesPath[i] << std::endl;
		filelinks.push_back(filesPath[i]);
	}

	if (twitterObj.uploadPictureFromFile(filelinks, Utils::cp1251_to_utf8(textStatus.c_str())))
	{
		twitterObj.getLastWebResponse(replyMsg);
		console() << "twitterClient: " << replyMsg.c_str() << std::endl;
		status = POST_READY;
		return;
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		console() << "twitterClient error: " << replyMsg.c_str() << std::endl;
		status = POST_ERROR;
		return;
	}
}

void Twitter::postPhotoTweetBase64(const std::string& textStatus, const std::string& filesPath)
{
	if (!isAuthFlowComplete)
		return;

	std::string replyMsg = "";

	DataSourceRef urlRequest = loadFile(filesPath);
	Buffer bf = Buffer(urlRequest);
	string strj = toBase64(bf);

	if (twitterObj.uploadPictureBase64(strj, Utils::cp1251_to_utf8(textStatus.c_str())))
	{
		twitterObj.getLastWebResponse(replyMsg);
		console() << "twitterClient: " << replyMsg.c_str() << std::endl;
		status = POST_READY;
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		console() << "twitterClient: " << replyMsg.c_str() << std::endl;
		status = POST_ERROR;
	}
}

void Twitter::update()
{
	switch (status)
	{
	case GET_AUTH_URL:
		getAuthUpdate();
		break;

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

void Twitter::getAuthUpdate()
{
	if (!authURL.empty())
	{
		if (mWebViewPtr)
		{
			mWebViewPtr->Resize(getBrowserWidth(), getBrowserHeight());
			mWebViewPtr->LoadURL(WebURL(WSLit(authURL.c_str())));
			mWebViewPtr->Focus();
		}
		status = WAITING_FOR_NETWORK;
	}
}

void Twitter::signInUpdate()
{
	if (!isAuthFlowComplete)
	{
		std::string queryString = chrome().convertToString(mWebViewPtr->url().query());
		std::string oauth_verifier = twitterObj.findOauthVerifier(queryString);

		if (!oauth_verifier.empty())
		{
			twitterObj.oAuthAccessToken(oauth_verifier);
			isAuthFlowComplete = true;
			post();
		}
		else if (!mWebViewPtr->IsLoading() && userRejectedAppOauth())
			status = USER_REJECT;
	}
}

bool Twitter::userRejectedAppOauth()
{
	string param = "document.documentElement.outerHTML";
	WebString html = mWebViewPtr->ExecuteJavascriptWithResult(WSLit(param.c_str()), Awesomium::WSLit("")).ToString();
	std::string htmlString = chrome().convertToString(html);

	size_t nPosStart = htmlString.find("oauth cancelled");

	return nPosStart != std::string::npos;
}

void Twitter::initChromium()
{
	status = GET_AUTH_URL;
	authURL.clear();

	mWebCorePtr = chrome().getWebCorePtr();
	mWebViewPtr = chrome().getWebViewPtr();
	loadingThread = ThreadRef(new boost::thread(&Twitter::initChromiumThread, this));

	chrome().clearCookies();
}

void Twitter::initChromiumThread()
{
	authURL = getAuthUrl();
}

std::string Twitter::getAuthUrl()
{
	isAuthFlowComplete = false;
	twitterObj.getOAuth().clearOAuth();

	twitterObj.getOAuth().setConsumerKey(SocialSettings::TWITTER_API_KEY);
	twitterObj.getOAuth().setConsumerSecret(SocialSettings::TWITTER_API_SECRET);

	std::string authUrl;
	twitterObj.oAuthRequestToken(authUrl, SocialSettings::TWITTER_OAUTH_CALLBACK);

	return authUrl;
}

void Twitter::logOut()
{
	isAuthFlowComplete = false;
	twitterObj.getOAuth().clearOAuth();
}

std::string  Twitter::getDefaultStatus() const
{
	return SocialSettings::TWITTER_STATUS_DEFAULT;
}

int Twitter::getBrowserWidth() const
{
	return 675;
}

int Twitter::getBrowserHeight() const
{
	return 360;
}

std::string Twitter::getPostingStatus() const
{
	return getDefaultStatus();
}

std::vector<std::string> Twitter::getUploadPhotoPathVec() const
{
	string path1 = "c:\\projects\\cinder_0.8.6_vc2012\\apps\\KUBIK\\Engine\\vc2012\\Debug\\data\\interface\\gamesDesign\\icons\\icon2.png";
	string path2 = "c:\\projects\\cinder_0.8.6_vc2012\\apps\\KUBIK\\Engine\\vc2012\\Debug\\data\\interface\\gamesDesign\\icons\\icon1.png";
	std::vector<std::string> filesPath;
	filesPath.push_back(path1);
	filesPath.push_back(path2);

	return filesPath;
}