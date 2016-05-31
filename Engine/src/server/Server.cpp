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
	if (currentRequest->success == RequestStatus::SUCCESS)
	{
		switch (currentRequest->getType())
		{
		case RequestType::LOGIN:
			loginSuccess();	
			logger().log("login!!!!!!!!!");
			break;

		case RequestType::PHOTO_UPLOAD:
		{
			PostPhotoPhotoBoothRequestRef postPhoto = static_pointer_cast<PostPhotoPhotoBoothRequest>(currentRequest);
			photoUploadSuccess(toString(postPhoto->photo_id), postPhoto->link);
			break;
		}

		case RequestType::CREATE_EVENT:
		{
			CreateEventRequestRef eventRequest = static_pointer_cast<CreateEventRequest>(currentRequest);
			createEventSuccess(eventRequest->name);
			break;
		}

		case RequestType::SOCIAL_TOKENS:
		{
			SocialTokenEventRef socialTokenRequest = static_pointer_cast<SocialTokenEvent>(currentRequest);
			socialTokensSuccess(socialTokenRequest->tokens);
			break;
		}

		case RequestType::SOCIAL_SHARE_TEXTS:
		{
			SocialShareTextsEventRef socialTextRequest = static_pointer_cast<SocialShareTextsEvent>(currentRequest);
			socialTextSuccess(socialTextRequest->texts);
			break;
		}
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
	auto response = Curl::post(request->url, request->getMapData());
	logger().log("login response ::  " + toString(response));
	isLogin = false;

	try
	{
		JsonTree data = JsonTree(response);

		if (data.hasChild("access_token"))
		{
			logger().log("LOGIN SUCCESS");

			access_token = data.getChild("access_token").getValue<string>();
			string token_type = data.getChild("token_type").getValue<string>();
			int expires_in = data.getChild("expires_in").getValue<int>();

			logger().log("access_token:::  " + access_token);
			logger().log("token_type:::  " + token_type);
			logger().log("expires_in:::  " + expires_in);

			isLogin = true;

			request->success = RequestStatus::SUCCESS;
		}
		else
		{
			request->success = RequestStatus::FAIL;
		}

		threadLoad = false;
	}
	catch (...)
	{

	}
	
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
	logger().log(response);

	if (RESPONSE_OK(response))
	{
		logger().log("PHOTO PRINT SUCCESS");
		request->success = RequestStatus::SUCCESS;
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::photoShare(int id, const string& type, int photo_id)
{
	PhotoShareRequestRef request = PhotoShareRequestRef(new PhotoShareRequest());
	request->setURL(SERVER + "/api/event/share");
	request->id = id;
	request->type = type;
	request->photo_id = photo_id;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadPhotoShare, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadPhotoShare(const PhotoShareRequestRef& request)
{
	string response = Curl::postStand(request->url, request->getMapData(), access_token);

	if (RESPONSE_OK(response))
	{
		logger().log("PHOTO SHARE SUCCESS");
		request->success = RequestStatus::SUCCESS;
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::createEvent(const string& name)
{
	CreateEventRequestRef request = CreateEventRequestRef(new CreateEventRequest());
	request->setURL(SERVER + "/api/create_event");
	request->name = name;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadCreateEvent, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadCreateEvent(const CreateEventRequestRef& request)
{
	string response = Curl::postStand(request->url, request->getMapData(), access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("EVENT CREATE SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::gameEnter(int appID)
{
	GameEventRef request = GameEventRef(new GameEvent());
	request->setURL(SERVER + "/api/event/game_enter");
	request->id = appID;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadGameEnter, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadGameEnter(const GameEventRef& request)
{
	string response = Curl::postStand(request->url, request->getMapData(), access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("Game Enter SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}


void Server::gamePass(int appID, int photo_id)
{
	GameEventRef request = GameEventRef(new GameEvent());
	request->setURL(SERVER + "/api/event/game_pass");
	request->id = appID;
	request->photo_id = photo_id;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadGamePass, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadGamePass(const GameEventRef& request)
{
	string response = Curl::postStand(request->url, request->getMapData(), access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("Game Pass SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::getSocialTokens()
{
	SocialTokenEventRef request = SocialTokenEventRef(new SocialTokenEvent());
	request->setURL(SERVER + "/api/settings/token");

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadGetSocialTokens, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadGetSocialTokens(const SocialTokenEventRef& request)
{
	string response = Curl::getStand(request->url, access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("threadGetSocialTokens SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::getSocialShareTexts()
{
	SocialShareTextsEventRef request = SocialShareTextsEventRef(new SocialShareTextsEvent());
	request->setURL(SERVER + "/api/app/text");

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadGetSocialShareTexts, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadGetSocialShareTexts(const SocialShareTextsEventRef& request)
{
	string response = Curl::getStand(request->url, access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("getSocialShareTexts SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
}

void Server::gameFail(int appID)
{
	GameEventRef request = GameEventRef(new GameEvent());
	request->setURL(SERVER + "/api/event/game_fail");
	request->id = appID;
	//request->photo_id = photo_id;

	if (checkLogin() && !threadLoad)
	{
		threadLoad = true;
		currentRequest = request;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadGameFail, this, request)));
	}
	else
	{
		//addToQue(request);
	}
}

void Server::threadGameFail(const GameEventRef& request)
{
	string response = Curl::postStand(request->url, request->getMapData(), access_token);

	if (RESPONSE_OK(response))
	{
		logger().log(response);
		request->success = RequestStatus::SUCCESS;
		logger().log("Game Fail SUCCESS");
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}

	threadLoad = false;
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
	string response = Curl::getStand(request->url, access_token);

	if (RESPONSE_OK(response))
	{
		JsonTree jTree = JsonTree(response);
		JsonTree data = jTree.getChild("data");
		auto event_title = data.getChild("event_title").getValue<string>();
		auto game_enter = data.getChild("game_enter").getValue<int>();
		auto photo_total = data.getChild("photo_total").getValue<int>();
		auto print_total = data.getChild("print_total").getValue<int>();
		auto social_total = data.getChild("social_total").getValue<int>();
		auto email_total = data.getChild("email_total").getValue<int>();
		auto cartridge_size = data.getChild("cartridge_size").getValue<int>();
		auto cartridge_current = data.getChild("cartridge_current").getValue<int>();

		logger().log(response);
		request->success = RequestStatus::SUCCESS;
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
	string response = Curl::postImageStand(request->url, request->base64Str, access_token);
	logger().log(response);

	if (RESPONSE_OK(response))
	{
		JsonTree data = JsonTree(response).getChild("data");
		
		request->link = data.getChild("link").getValue<string>();
		request->photo_id = data.getChild("photo_id").getValue<int>();	
		request->success = RequestStatus::SUCCESS;
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

	if (RESPONSE_OK(response))
	{
		request->success = RequestStatus::SUCCESS;
	}
	else
	{
		request->success = RequestStatus::FAIL;
	}
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

bool Server::RESPONSE_OK(const string& response)
{
	JsonTree jTree = JsonTree(response);

	if (toString(response) != "")
	{
		try
		{
			JsonTree jTree = JsonTree(response);
			if (jTree.hasChild("meta"))
			{
				auto code = jTree.getChild("meta").getChild("code").getValue<int>();
				if (code == 200)
				{
					logger().log("response success");
					return true;
				}
			}
		}
		catch (...)
		{

		}
	}
	return false;
}

