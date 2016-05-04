#include "server/Server.h"
#include "VirtualKeyboard.h"
#include "Curl.h"
#include "config/Paths.h"

using namespace kubik;
using namespace kubik::config;
using namespace mndl::curl;
using namespace std;
using namespace ci;
using namespace ci::app;

void Server::init()
{
	/*updateCon =*/ App::get()->getSignalUpdate().connect(std::bind(&Server::update, this));

}

void Server::update()
{
	/*if (photoIsPosted)
	{
		photoIsPosted = false;
		photoUploadSuccess();
	}*/
}

void Server::login()
{
	AuthRequestRef request = AuthRequestRef(new AuthRequest());
	request->url = SERVER + "/oauth/access_token";

	if (!threadLoad)
	{
		threadLoad = true;	
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadLogin, this, request)));
	}
}

void Server::threadLogin(const AuthRequestRef& request)
{
	auto answer = Curl::post(request->url, request->getMapData());
	logger().log("login response ::  " + toString(answer));
	isLogin = false;

	if (toString(answer) != "")
	{
		try
		{
			JsonTree jTree = JsonTree(answer);
			if (jTree.hasChild("access_token"))
			{
				access_token	  = jTree.getChild("access_token").getValue<string>();
				string token_type = jTree.getChild("token_type").getValue<string>();
				int expires_in	  = jTree.getChild("expires_in").getValue<int>();

				logger().log("access_token:::  " + access_token);
				logger().log("token_type:::  " + token_type);
				logger().log("expires_in:::  " + expires_in);

				isLogin = true;
				request->success = RequestStatus::SUCCESS;
			}
		}
		catch (...)
		{
			
		}
	}

	request->success = RequestStatus::FAIL;
	threadLoad = false;
}

void Server::standInfo()
{
	string REQUEST = "/api/stand";
	auto answer = Curl::getStand(SERVER + REQUEST, access_token);
	logger().log("answer::  " + toString(answer));
}

void Server::postPhoto(const string& path)
{
	auto request = PostPhotoPhotoBoothRequestRef(new PostPhotoPhotoBoothRequest());
	request->setPath(path);
	request->init();
	request->setURL(SERVER + "/api/photo_upload/2/" + toString(request->getHeight()));	

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;	
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadPostPhoto, this, request)));
	}	
	else
	{
		//addToQue(request);
	}
}

void Server::threadPostPhoto(const PostPhotoPhotoBoothRequestRef& request)
{
	string jSonstr = Curl::postImageStand(request->url, request->base64Str, access_token);
	logger().log(jSonstr);

	if (jSonstr.length() >= 1)
	{
		try
		{
			JsonTree jTree = JsonTree(jSonstr);
			if (jTree.hasChild("data"))
			{
				auto data = jTree.getChild("data");
				if (data.hasChild("link"))
				{
					link = data.getChild("link").getValue<string>();
					photo_id = data.getChild("photo_id").getValue<int>();	
					request->success = RequestStatus::SUCCESS;
				}			
			}			
		}
		catch (...)
		{
			
		}
	}

	request->success = RequestStatus::FAIL;
}

void Server::sendToEmails(int appID, int photoID, const string& emails)
{
	logger().log("id  " + toString(appID));
	logger().log("photo_id  " + toString(photoID));
	logger().log("to  " + toString(emails));

	auto request = SendToEmailsRequestRef(new SendToEmailsRequest());
	request->photo_id = photoID;
	request->emails = emails;
	request->app_id = appID;
	request->url = SERVER + "/api/event/email";

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::sendToEmailsThread, this, request)));
	}
	else
	{
		//addToQue(request);
	}	
}

void Server::sendToEmailsThread(const SendToEmailsRequestRef& request)
{	
	auto response = Curl::postStand(request->url, request->getMapData(), access_token);
	request->success = RequestStatus::SUCCESS;
	threadLoad = false;
}

bool Server::checkLogin()
{
	bool temp = isLogin;

	if (!isLogin)
	{
		login();
	}

	return temp;
}

void Server::addToQue(const RequestRef& request)
{

}