#include "Twitter.h"

using namespace kubik::config;
using namespace kubik;
using namespace std;

Twitter::Twitter():isAuthFlowComplete(false)
{ 
	availableArea = (Rectf(0.f, 0.f, 897.f, 538.f));	
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
		//postTextTweet(textstatus);
		status = GET_AUTH_URL;
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
		else if (status == GET_AUTH_URL)
			gettingAuthURLComplete();
	}
}

bool Twitter::authorize(const string& login, const string& password)
{
	isAuthFlowComplete = false;

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

	//if (SocialSettings::TWITTER_TOKEN_KEY.size() && SocialSettings::TWITTER_TOKEN_SECRET.size())
   //{
   //    /* If we already have these keys, then no need to go through auth again */
	//   twitterObj.getOAuth().setOAuthTokenKey(SocialSettings::TWITTER_TOKEN_KEY);
	///   twitterObj.getOAuth().setOAuthTokenSecret(SocialSettings::TWITTER_TOKEN_SECRET);
   /// }
    //else
    {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );
		console()<<" oath ---------------------------"<<authUrl<<std::endl;
		return true;

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

std::string Twitter::getAuthUrl()
{	
	twitterObj.getOAuth().setConsumerKey(SocialSettings::TWITTER_API_KEY);
	twitterObj.getOAuth().setConsumerSecret(SocialSettings::TWITTER_API_SECRET);
	std::string authUrl;
	console() << "auth url:::   " << authUrl << endl;
	twitterObj.oAuthRequestToken(authUrl);

	return authUrl;
}

void Twitter::update()
{	
	SocShare::update();

	if (isAuthFlowComplete) 
		return;

	//std::string userName("ferry.is.very@gmail.com");
	//std::string passWord("qwerty12+");

	//twitterObj.setTwitterUsername(userName);
	//twitterObj.setTwitterPassword(passWord);
	//twitterObj.getOAuth().setConsumerKey(SocialSettings::TWITTER_API_KEY);
	//twitterObj.getOAuth().setConsumerSecret(SocialSettings::TWITTER_API_SECRET);

	//std::string authUrl;
	//twitterObj.oAuthRequestToken(authUrl);
	//console() << " oath ---------------------------" << authUrl << std::endl;
	//
	//twitterObj.oAuthHandlePIN(authUrl);//!!!!!!!!!!!!!!!
	//isAuthFlowComplete = true;



	if (status != WAITING_FOR_NETWORK)
		return;

	//char anchr[1024];
	//mWebViewPtr->url().query().ToUTF8(anchr, 1024);
	//string anchString(anchr);
	//console() << "::::-------------------  query " << anchString << endl;

	//mWebViewPtr->url().spec().ToUTF8(anchr, 1024);
	//anchString = anchr;
	//console() << "::::-------------------  spec " << anchString << endl;

	//anchString = anchr;
	//console() << "::::-------------------  scheme " << anchString << endl;
	//
	string param = "document.documentElement.outerHTML";
//	param = "document.getElementsByTagName('html')[0].innerHTML";
	//param = "document.getElementsByTagName('input')";
	
	WebString html = mWebViewPtr->ExecuteJavascriptWithResult(WSLit(param.c_str()), Awesomium::WSLit("")).ToString();
	
	char html_[100024];
	html.ToUTF8(html_, 100024);
	string htmlString(html_);
	//JsonTree configJSON = JsonTree(htmlString);
	//console() << "::::-------------------  html_ " << htmlString << endl;
	//console() << "::::-------------------  mWebViewPtr->focused_element_type() " << mWebViewPtr->Copy() << endl;

	//clipboar
	
	int nPosStart = htmlString.find("<code>");	
	
	if (std::string::npos != nPosStart)
	{
		int nPosEnd = htmlString.substr(nPosStart + 6).find("</code>");

		if (std::string::npos != nPosEnd)
		{
			string pinCodeVal = htmlString.substr(nPosStart + 6, nPosEnd);

			console() <<"::::  "<< pinCodeVal << endl;
			authorize("yurikblech@gmail.com", "Metalcorehero88");
			//std::string userName("yurikblech@gmail.com");
			//std::string passWord("Metalcorehero88");
			//mWebViewPtr->LoadURL(WebURL(WSLit(getAuthUrl().c_str())));
		/*	std::string userName("ferry.is.very@gmail.com");
			std::string passWord("qwerty12+");

			twitterObj.setTwitterUsername(userName);
			twitterObj.setTwitterPassword(passWord);
			twitterObj.getOAuth().setConsumerKey(SocialSettings::TWITTER_API_KEY);
			twitterObj.getOAuth().setConsumerSecret(SocialSettings::TWITTER_API_SECRET);
			twitterObj.getOAuth().setOAuthPin(pinCodeVal);
			twitterObj.oAuthAccessToken();

			isAuthFlowComplete = true;

			postTextTweet("kubik test");*/

			console() << ":::: posted :::: "  << endl;
		}		
	}
}

void Twitter::updatePopupPosition()
{
	if (mWebTexture)
		popupPosition = Vec2f(0.5f * (getWindowWidth() - mWebTexture.getWidth()), 166.0f);
	else
		popupPosition = Vec2f::zero();
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

int Twitter::getBrowserWidth()
{
	return 675;
}

int Twitter::getBrowserHeight()
{
	return 360;
}