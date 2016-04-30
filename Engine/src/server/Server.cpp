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

void Server::login()
{
	if (!threadLoad)
	{
		threadLoad = true;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadLogin, this)));
	}
}

void Server::update()
{
	if (photoIsPosted)
	{
		photoIsPosted = false;
		photoUploadSuccess();
	}
}

void Server::threadLogin()
{
	string REQUEST = "/oauth/access_token";

	map<string, string> data;
	data.insert(pair<string, string>("username", user_name));
	data.insert(pair<string, string>("password", password));
	data.insert(pair<string, string>("grant_type", grant_type));
	data.insert(pair<string, string>("client_id", client_id));
	data.insert(pair<string, string>("client_secret", client_secret));

	auto answer = Curl::post(SERVER + REQUEST, data);
	logger().log("answer::  " + toString(answer));
	isLogin = false;

	if (toString(answer) != "")
	{
		try
		{
			JsonTree jTree = JsonTree(answer);
			if (jTree.hasChild("access_token"))
			{
				access_token = jTree.getChild("access_token").getValue<string>();
				string token_type = jTree.getChild("token_type").getValue<string>();
				int expires_in = jTree.getChild("expires_in").getValue<int>();

				logger().log("access_token:::  " + access_token);
				logger().log("token_type:::  " + token_type);
				logger().log("expires_in:::  " + expires_in);

				isLogin = true;
			}
		}
		catch (...)
		{
			
		}
	}

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
	if (!threadLoad)
	{
		errorCount = 0;
		threadLoad = true;
		photoIsPosted = false;
		link = "";
		photo_id = -1;

		fs::path path1 = Paths::getPhotoTemplateToServerPath(0);
		DataSourceRef urlRequest = loadFile(path1);
		auto loadedTex = gl::Texture(loadImage(urlRequest));

		Buffer bf = Buffer(urlRequest);
		photoObj.base64Str = toBase64(bf);
		photoObj.height = loadedTex.getHeight();

		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::threadPostPhoto, this)));
	}	
}

void Server::threadPostPhoto()
{
	if (isLogin)
	{
		string REQUEST = "/api/photo_upload/2/" + toString(photoObj.height);
		string jSonstr = Curl::postImageStand(SERVER + REQUEST, photoObj.base64Str, access_token);

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
						photoIsPosted = true;
						
					}			
				}
				else
				{
					if (errorCount++ > 2)
					{
						threadLoad = false;
						photoUploadError();
						return;
					}

					threadLogin();
					threadPostPhoto();
				}
			}
			catch (...)
			{
				if (errorCount++ > 2)
				{
					threadLoad = false;
					photoUploadError();
					return;
				}

				threadLogin();
				threadPostPhoto();
			}
		}

		threadLoad = false;
	}
	else
	{
		if (errorCount++ > 2)
		{
			threadLoad = false;
			//photoUploadError();
			return;
		}

		threadLogin();
		threadPostPhoto();
	}
}



void Server::sendToEmails(int appID, int photoID, const string& emails)
{
	logger().log("id  " + toString(appID));
	logger().log("photo_id  " + toString(photoID));
	logger().log("to  " + toString(emails));

	if (!threadLoad)
	{
		threadLoad = true;
		mediaLoadThread = ThreadRef(new boost::thread(bind(&Server::sendToEmailsThread, this, appID, photoID, emails)));
	}
}

void Server::sendToEmailsThread(int appID, int photoID, const string& emails)
{
	string REQUEST = "/api/event/email";
	map<string, string> data;
	data.insert(pair<string, string>("id", toString(appID)));
	data.insert(pair<string, string>("photo_id", toString(photoID)));
	data.insert(pair<string, string>("to", emails));	

	auto answer = Curl::postStand(SERVER + REQUEST, data, access_token);
}

std::string Server::getPhotoLink() const
{
	return link;
}

int Server::getPhotoID() const
{
	return photo_id;
}