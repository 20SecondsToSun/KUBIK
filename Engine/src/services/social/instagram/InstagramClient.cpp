#include "instagram/InstagramClient.h"
#include "Tools/Logger.h"


using namespace std;
using namespace ci;
using namespace ci::app;
using namespace instagram;
using namespace mndl::curl;

InstagramClient::InstagramClient(const string& clientID)
	:clientID(clientID),
	_loading(false),
	_needSynch(false),
	_noMore(false)
{

}

////////////////////////////////////////////////////////////////////////////
//
//					LOAD POPULAR
//
////////////////////////////////////////////////////////////////////////////


void InstagramClient::loadPopular(int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadPopular, this, count)));
	}
}

void InstagramClient::_loadPopular(int count)
{
	string request = API::POPULAR + "?" + clientIDParam() + "&COUNT=" + to_string(count);
	loadMediaRequest(request);
}

////////////////////////////////////////////////////////////////////////////
//
//					LOAD BY TAG
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadTagMedia(const string& tagName, int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadTagMedia, this, tagName, count)));
	}
}

void InstagramClient::_loadTagMedia(const string& tagName, int count)
{
	string tagUTF = Utils::cp1251_to_utf8(tagName.c_str());
	string request = API::TAGS + tagUTF + "/" + API::MEDIA_RECENT + "?" + clientIDParam() + "&COUNT=" + to_string(count);
	loadMediaRequest(request);
}

////////////////////////////////////////////////////////////////////////////
//
//					USER LOAD 
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadUserMedia(const string& tagName, int count)
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadUserMedia, this, tagName, count)));
	}
}

void InstagramClient::_loadUserMedia(const string& userName, int count)
{
	string tagUTF = Utils::cp1251_to_utf8(userName.c_str());
	string request = API::USERS + API::SEARCH + "?q=" + tagUTF + "&" + clientIDParam();
	loadUsersRequest(request, userName);
}

void InstagramClient::loadUsersRequest(const string& request, const string& _userName)
{
	string json = Curl::get(request);
	userResponse.parse(json);

	auto data = userResponse.getData();	
	
	if (data.empty())
	{	
		lastCode = API::USER_NOT_EXIST;
		_needSynch = true;
		_loading = false;			
	}
	else
	{
		lastCode = userResponse.getCode();

		for (auto it : data)
		{
			kubik::logger().log(it.getUsername());
		}

		auto it = std::find_if(std::begin(data), std::end(data),
			[&](const User& user){ return user.getUsername() == _userName; });

		User firstUser;

		if (data.end() == it)
		{
			firstUser = data.front();
		}
		else
		{
			firstUser = *it;
		}

		loadUserPhotos(firstUser.getID());		
	}
}

void InstagramClient::loadUserPhotos(const string& userID, int count)
{
	string request = API::USERS + userID + "/" + API::MEDIA_RECENT + "?" + clientIDParam() + "&COUNT=" + to_string(count);
	loadMediaRequest(request);
}

string InstagramClient::clientIDParam() const
{
	return API::CLIENT_ID + "=" + clientID;
}

////////////////////////////////////////////////////////////////////////////
//
//					PAGINATION
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadNextMedia()
{
	if (canLoad())
	{
		setupLoadThread();
		mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadNextMedia, this)));
	}
}

void InstagramClient::_loadNextMedia()
{
	if (lastMediaResponse.getPagination().hasNextURL())
	{
		auto url = lastMediaResponse.getPagination().getNextURL();		
		loadMediaRequest(url);
	}
	else
	{
		_needSynch = false;		
		_noMore	   = true;
		_loading   = false;
	}
}

void InstagramClient::loadMediaRequest(const string& request)
{
	string json = Curl::get(request);
	lastMediaResponse.parse(json);
	loadImages();

	lastCode = lastMediaResponse.getCode();
	_needSynch = true;
	_loading   = false;
}

void InstagramClient::setupLoadThread()
{
	synchImages.clear();
	_loading  = true;
	updateCon = App::get()->getSignalUpdate().connect(std::bind(&InstagramClient::update, this));
	startLoadEvent();
}

void InstagramClient::update()
{
	if (!_loading)
	{
		if (_needSynch)
		{
			updateCon.disconnect();
			synchEvent();
		}
		else if (_noMore)
		{
			updateCon.disconnect();
			noMoreEvent();
		}	
	}
}

////////////////////////////////////////////////////////////////////////////
//
//					LOADING
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadImages()
{
	list<InstagramMedia> mediaList = lastMediaResponse.getData();

	synchImages.clear();

	for (auto image : mediaList)
	{
		ImageGraphic imageGr;
		auto url = image.getImagesList().getLowResolution().getURL();
		kubik::logger().log(url);
		imageGr.setLowResURL(url);
		imageGr.setStandartResURL(image.getImagesList().getStandardResolution().getURL());
		imageGr.setSize(THUMB_SIZE);
		synchImages.push_back(imageGr);
	}
}

void InstagramClient::killLoad()
{
	ci::app::console() << "TRY TO KILL!!!!!!!!!!!!!!!" << endl;

	if (_loading)
	{
		ci::app::console() << "join!!!!!!!!!!!!!!!" << endl;

		if (mediaLoadThread && mediaLoadThread->joinable())
		{
			//mediaLoadThread->interrupt();
			mediaLoadThread->join();
		}

		//if (mediaLoadThread && mediaLoadThread->joinable())
		//	mediaLoadThread->join();
	}

	_loading = false;
	_needSynch = false;
	ci::app::console() << "KILLED!!!!!!!!!!!!!!!" << endl;
}

////////////////////////////////////////////////////////////////////////////
//
//					GETTERS
//
////////////////////////////////////////////////////////////////////////////

bool InstagramClient::isLoading() const
{
	return _loading;
}

bool InstagramClient::needSynch() const
{
	return _needSynch;
}

void InstagramClient::setSynch(bool val)
{
	_needSynch = val;
}

std::vector<ImageGraphic> InstagramClient::getImages() const
{
	return synchImages;
}

bool InstagramClient::canLoad() const
{
	return !_loading && !_needSynch;
}

int InstagramClient::getLastCode() const
{
	return lastCode;
}

bool InstagramClient::userPrivate() const
{
	return lastCode == 400;
}

bool InstagramClient::userNotExist() const
{
	return lastCode == API::USER_NOT_EXIST;
}

bool InstagramClient::userNotHavePhotos() const
{
	return synchImages.empty();
}

bool InstagramClient::noHashtagPhotos() const
{
	return synchImages.empty();
}