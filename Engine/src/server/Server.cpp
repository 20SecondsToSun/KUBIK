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

	if (currentRequest->success == RequestStatus::SUCCESS)
	{
		switch (currentRequest->getType())
		{
		case RequestType::LOGIN:
			loginSuccess();	
			logger().log("login!!!!!!!!!");
			break;

		case RequestType::PHOTO_UPLOAD:
			PostPhotoPhotoBoothRequestRef currentRequest = static_cast<PostPhotoPhotoBoothRequestRef>(currentRequest);			
			photoUploadSuccess(toString(currentRequest->photo_id), currentRequest->link);
			break;
		}

		currentRequest->setHandled();

	}
}

void Server::login()
{
	AuthRequestRef request = AuthRequestRef(new AuthRequest());
	request->setURL(SERVER + "/oauth/access_token");

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
			request->success = RequestStatus::FAIL;
		}
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	
	threadLoad = false;
}

void Server::photoPrint(int appID, int photo_id, const string& photo_url, const string& hashtag)
{
	auto request = PhotoPrintRequestRef(new PhotoPrintRequest());	
	request->setURL(SERVER + "/api/event/print");
	request->appID = appID;
	request->photo_id = photo_id;
	request->photo_url = photo_url;
	request->hashtag = hashtag;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadPhotoPrint, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadPhotoPrint(const PhotoPrintRequestRef& request)
{
	auto response = Curl::postStand(request->url, request->getMapData(), access_token);
}

void Server::standInfo()
{
	StandInfoRequestRef request = StandInfoRequestRef(new StandInfoRequest());
	request->setURL(SERVER + "/api/stand");

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadStandInfo, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadStandInfo(const StandInfoRequestRef& request)
{
	string answer = Curl::getStand(request->url, access_token);

	if (toString(answer) != "")
	{
		try
		{
			JsonTree jTree = JsonTree(answer);
			if (jTree.hasChild("meta"))
			{
				auto code = jTree.getChild("meta").getChild("code").getValue<int>();
				if (code == 200)
				{
					JsonTree data = jTree.getChild("data");
					auto event_title = data.getChild("event_title").getValue<string>();
					auto game_enter = data.getChild("game_enter").getValue<int>();
					auto photo_total = data.getChild("photo_total").getValue<int>();
					auto print_total = data.getChild("print_total").getValue<int>();
					auto social_total = data.getChild("social_total").getValue<int>();
					auto email_total = data.getChild("email_total").getValue<int>();
					auto cartridge_size = data.getChild("cartridge_size").getValue<int>();
					auto cartridge_current = data.getChild("cartridge_current").getValue<int>();

					logger().log(answer);
					logger().log("cartridge_size" + toString(cartridge_size));
					request->success = RequestStatus::SUCCESS;
				}
				else
				{
					request->success = RequestStatus::FAIL;
				}				
			}
		}
		catch (...)
		{
			request->success = RequestStatus::FAIL;
		}
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;	
}

void Server::postPhoto(const string& path, const string& appID)
{
	auto request = PostPhotoPhotoBoothRequestRef(new PostPhotoPhotoBoothRequest());
	request->setPath(path);
	request->init();
	request->setURL(SERVER + "/api/photo_upload/" + toString(appID) + "/" + toString(request->getHeight()));

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
					request->link = data.getChild("link").getValue<string>();
					request->photo_id = data.getChild("photo_id").getValue<int>();
					request->success = RequestStatus::SUCCESS;
				}			
			}			
		}
		catch (...)
		{
			request->success = RequestStatus::FAIL;
		}
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;	
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
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadSendToEmails, this, request)));
	}
	else
	{
		//addToQue(request);
	}	
}

void Server::threadSendToEmails(const SendToEmailsRequestRef& request)
{	
	auto response = Curl::postStand(request->url, request->getMapData(), access_token);
	logger().log(response);
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