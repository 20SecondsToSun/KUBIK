#include "instagram/InstagramClient.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace instagram;
using namespace mndl::curl;

InstagramClient::InstagramClient(const string& clientID)
	:clientID(clientID), _loading(false), _needSynch(false), _noMore(false)
{

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
	string request = API::TAGS + tagName + "/" +
		API::MEDIA_RECENT + "?" +
		API::CLIENT_ID + "=" + clientID +
		"&COUNT=" + to_string(count);

	loadMediaRequest(request);
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
	if(lastMediaResponse.getPagination().hasNextURL())
		loadMediaRequest(lastMediaResponse.getPagination().getNextURL());
	else
	{
		console() << "no more::::::::::::::  " << endl;
		_needSynch = false;
		_loading = false;
		_noMore = true;
	}
}

void InstagramClient::loadMediaRequest(const string& request)
{
	string json = Curl::get(request);
	lastMediaResponse.parse(json);
	loadImages();

	_needSynch = true;
	_loading = false;
}

void InstagramClient::setupLoadThread()
{	
	_loading = true;
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

void InstagramClient::loadImages()
{
	list<InstagramMedia> mediaList = lastMediaResponse.getData();

	ci::app::console() << "loaded InstagramMedia ::::  " << mediaList.size() << endl;

	synchImages.clear();

	for (auto image : mediaList)
	{
		ImageGraphic imageGr;
		imageGr.setLowResURL(image.getImagesList().getLowResolution().getURL());
		imageGr.setStandartResURL(image.getImagesList().getStandardResolution().getURL());
		imageGr.setSize(THUMB_SIZE);
		synchImages.push_back(imageGr);
	}
}

void InstagramClient::killLoad()
{
	if (_loading)
	{
		if (mediaLoadThread && mediaLoadThread->joinable())
			mediaLoadThread->join();
	}

	_loading = false;
	_needSynch = false;
	ci::app::console() << "KILLED!!!!!!!!!!!!!!!" << endl;
}