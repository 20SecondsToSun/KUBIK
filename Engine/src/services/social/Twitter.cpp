#include "Twitter.h"

using namespace kubik::config;
using namespace kubik;
using namespace std;

Twitter::Twitter() 
	:isAuthFlowComplete(false)
{ 
	
};

void Twitter::authorizePost(const std::string& login, const std::string& password, const std::string& textstatus)
{
	if (status == POSTING)
		return;

	status = POSTING;
	postingStart();

	loadingSignal = App::get()->getSignalUpdate().connect(bind(&Twitter::waitLoadingComplete, this));
	loadingThread = ThreadRef(new boost::thread(&Twitter::posting, this, login, password, textstatus));
}

void Twitter::posting(const std::string& login, const std::string& password, const std::string& textstatus)
{
	isAuthFlowComplete = false;

	try
	{
		authorize(login, password);
		postTextTweet(textstatus);
		status = POST_READY;
	}
	catch (...)
	{
		status = POST_ERROR;		
	}	
}

void Twitter::waitLoadingComplete()
{
	if (status != POSTING)
	{
		loadingThread->join();
		loadingSignal.disconnect();

		if (status == POST_ERROR)
			postingError();
		else if (status == POST_READY)
			postingComplete();
	}
}

bool Twitter::authorize(const string& login, const string& password)
{
	std::string userName( login );
    std::string passWord( password );	
   
    std::string tmpStr, tmpStr2;
    std::string replyMsg;
    //char tmpBuf[1024];

    /* Set twitter username and password */
    twitterObj.setTwitterUsername( userName );
    twitterObj.setTwitterPassword( passWord );

	 /* OAuth flow begins */

    /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
	twitterObj.getOAuth().setConsumerKey(SocialSettings::TWITTER_API_KEY);
	twitterObj.getOAuth().setConsumerSecret(SocialSettings::TWITTER_API_SECRET);

	if (SocialSettings::TWITTER_TOKEN_KEY.size() && SocialSettings::TWITTER_TOKEN_SECRET.size())
   {
       /* If we already have these keys, then no need to go through auth again */
	   twitterObj.getOAuth().setOAuthTokenKey(SocialSettings::TWITTER_TOKEN_KEY);
	   twitterObj.getOAuth().setOAuthTokenSecret(SocialSettings::TWITTER_TOKEN_SECRET);
    }
    else
    {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );
		console()<<" oath ---------------------------"<<authUrl<<std::endl;

		 twitterObj.oAuthHandlePIN( authUrl );//!!!!!!!!!!!!!!!



        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();


		string myOAuthAccessTokenKey, myOAuthAccessTokenSecret;
		/* Step 5: Now, save this access token key and secret for future use without PIN */
		twitterObj.getOAuth().getOAuthTokenKey(myOAuthAccessTokenKey);
		twitterObj.getOAuth().getOAuthTokenSecret(myOAuthAccessTokenSecret);

		console() << "::::  " << myOAuthAccessTokenKey << endl;
		console() << "::::  " << myOAuthAccessTokenSecret << endl;

		twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
		twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);

    }
    /* OAuth flow ends */

	 /* Account credentials verification */
    if( twitterObj.accountVerifyCredGet() )
    {
        twitterObj.getLastWebResponse( replyMsg );
		console()<<"MESSAGE  ::"<<replyMsg.c_str()<<std::endl;   
		isAuthFlowComplete = true;
		return true;
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
		console()<<"ERROR  ::"<<replyMsg.c_str()<<std::endl;
		//throw
		return false;       
    }
}

void Twitter::postTextTweet(const std::string& status)
{
	if (isAuthFlowComplete)
	{
		std::string replyMsg = "";

		if (!twitterObj.statusUpdate(Utils::cp1251_to_utf8(status.c_str())))
		{
			twitterObj.getLastCurlError(replyMsg);
			//throw
		}

		twitterObj.getLastWebResponse(replyMsg);
	} 	
}

bool Twitter::postPhotoTweet(const std::string& status, const std::vector<std::string>& filesPath)
{
	if (!isAuthFlowComplete) return false;

	int max_media_per_upload;
	std::string replyMsg = "";

	if( twitterObj.getTwitterConfiguration())
    {
        twitterObj.getLastWebResponse( replyMsg );
		try 
		{
			JsonTree jTree = JsonTree(replyMsg);
			max_media_per_upload = atoi(jTree.getChild("max_media_per_upload").getValue().c_str());	
		}
		catch(...){};
	}
	else
    {
        twitterObj.getLastCurlError( replyMsg );
        console()<<"twitterClient:: twitCurl::statusUpdate error: "<< replyMsg.c_str()<<std::endl;
		return false;
    }
	
	vector<string> filelinks;

	for (int i = 0; i < max_media_per_upload; i++)	
	{
		console()<<"path ::  "<<filesPath[i]<<std::endl;
		filelinks.push_back(filesPath[i]);	
	}
	
	if( twitterObj.uploadPictureFromFile(filelinks,  Utils::cp1251_to_utf8(status.c_str()) ))
    {
        twitterObj.getLastWebResponse( replyMsg );
        console()<<"twitterClient:: twitCurl:: statusUpdate web response: "<< replyMsg.c_str()<<std::endl;
		return true;
    }
	else
    {
        twitterObj.getLastCurlError( replyMsg );
        console()<<"twitterClient:: twitCurl:: statusUpdate error: "<< replyMsg.c_str()<<std::endl;
		return false;
    }
}
bool Twitter::postPhotoTweetBase64(const std::string& status, const std::string& filesPath)
{
	if (!isAuthFlowComplete) return false;
	std::string replyMsg = "";

	
	DataSourceRef urlRequest =	loadFile( filesPath);	
	Buffer bf = Buffer(urlRequest);
	string strj = toBase64(bf) ;

	if( twitterObj.uploadPictureBase64(strj,  Utils::cp1251_to_utf8(status.c_str()) ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        console()<<"twitterClient:: twitCurl::statusUpdate web response: "<< replyMsg.c_str()<<std::endl;
		return true;
    }
	else
    {
        twitterObj.getLastCurlError( replyMsg );
        console()<<"twitterClient:: twitCurl::statusUpdate error: "<< replyMsg.c_str()<<std::endl;
		return false;
    }
}

const char * Twitter::getAuthUrl()
{	
	return SocialSettings::TWITTER_AUTH_URL.c_str();
}

void Twitter::logOut()
{
	isAuthFlowComplete = false;
	login = password = "";	
}

string  Twitter::getDefaultStatus()
{
	return SocialSettings::TWITTER_STATUS_DEFAULT;
}