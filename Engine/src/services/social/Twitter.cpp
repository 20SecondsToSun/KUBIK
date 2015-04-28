#include "Twitter.h"

using namespace kubik::twParams;
using namespace kubik::config;
using namespace kubik;
using namespace std;

bool Twitter::authorize(const string& login, const string& password)
{
	std::string userName( login );
    std::string passWord( password );	
   
    std::string tmpStr, tmpStr2;
    std::string replyMsg;
    char tmpBuf[1024];

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
		return false;       
    }
}

bool Twitter::postTextTweet(const std::string& status)
{
	if (!isAuthFlowComplete) return false;

	std::string replyMsg = "";

	if( twitterObj.statusUpdate(  Utils::cp1251_to_utf8(status.c_str())  ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        console()<<"twitterClient:: twitCurl::statusUpdate web response: "<< replyMsg.c_str()<<std::endl;
		return true;
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        console()<<"twitterClient:: twitCurl1111::statusUpdate error: "<< replyMsg.c_str()<<std::endl;
		return false;
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

void Twitter::post()
{
	//serverThread = shared_ptr<thread>( new thread( bind( &Twitter::twitterPostThread, this ) ) );	
}

void Twitter::twitterPostThread()
{
	/*ci::ThreadSetup threadSetup; // instantiate this if you're talking to Cinder from a secondary thread	

	bool status;

	if (isAuthFlowComplete == false)
	{
		status = init(login, password);

		if (status == false) 
		{
			response = SocialPostError;
			//serverHandler();
			return;
		}
	}

	status =  false;

	switch (type)
	{
		case TEXT_STATUS:
			status = postTextTweet(textStatus);			
		break;

		case PHOTO_STATUS:
			status =  postPhotoTweet(textStatus, photosVector);			
		break;

		default:
		break;
	}

	if (status)
	{
		response = SocialPostOk;	
	}
	else
	{
		response = SocialPostError;
	}

	serverThread->detach();	

	// notificate main thread that twitter finished
	serverHandler();*/
}
const char * Twitter::getAuthUrl()
{	
	return twitterAuthURL.c_str();
}

void Twitter::logOut()
{
	isAuthFlowComplete = false;
	login = password = "";	
}

string  Twitter::getDefaultStatus()
{
	return STATUS_DEFAULT;
}