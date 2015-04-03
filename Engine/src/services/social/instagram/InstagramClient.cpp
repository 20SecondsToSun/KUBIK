#include "instagram/InstagramClient.h"

using namespace std;
using namespace ci;
using namespace instagram;
using namespace mndl::curl;

InstagramClient::InstagramClient(const string& clientID)
	:clientID(clientID), _loading(false),
	loadImagesImmediately(true), lastHiResIndex(0), kill(false)
{

}

/*void loadPopularMedias()
{
string request = API::POPULAR + "?" + API::CLIENT_ID + "=" + clientID;
console()<<"request-------:  "<<request<<endl;

string json    =  Curl::get(request);
JsonTree data  = JsonTree(json).getChild("data");

medialist.clear();

for(auto it : data)
medialist.push_back(parseImagesList(it));

loadImages();
}

void userSearch(string name)
{
string request = API::USER_SEARCH + "?" + API::QUERY + "=" + name + "&" + API::CLIENT_ID + "=" + clientID;
console()<<"request-------:  "<<request<<endl;

string json	   =  Curl::get(request);
JsonTree data  = JsonTree(json).getChild("data");

userList.clear();
for(auto it : data)
{
User user;
user.setID(it.getChild("id").getValue<int>());
userList.push_back(user);
}
console()<<"userList-------:  "<<userList.size()<<endl;
}

void loadUserFeed(int id)
{
string request = API::USERS + to_string(id) + "/" + API::MEDIA_RECENT + "?" + API::CLIENT_ID + "=" + clientID + "&count=10";
console()<<"request-------:  "<<request<<endl;
string json =  Curl::get(request);
JsonTree data  = JsonTree(json).getChild("data");
console()<<"----------------:  "<<endl;
JsonTree pagination  = JsonTree(json).getChild("pagination");

console()<<"pagination-------:  "<<pagination<<endl;

medialist.clear();
for(auto it : data)
medialist.push_back(parseImagesList(it));

console()<<"medialist-------:  "<<medialist.size()<<endl;

loadImages();
}

void tagSearch(string tagName)
{
string request = API::TAGS + API::SEARCH + "?" + API::QUERY + "=" + tagName + "&" + API::CLIENT_ID + "=" + clientID;
console()<<"request-------:  "<<request<<endl;
string json =  Curl::get(request);
JsonTree data  = JsonTree(json).getChild("data");
console()<<"----------------:  "<<data<<endl;
}
*/



////////////////////////////////////////////////////////////////////////////
//
//					LOAD BY TAG
//
////////////////////////////////////////////////////////////////////////////

void InstagramClient::loadTagMedia(const string& tagName, int count)
{
	if (_loading) return;

	_loading = true;
	mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadTagMedia, this, tagName, count)));
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
	if (_loading) return;

	_loading = true;
	mediaLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadNextMedia, this)));
}

void InstagramClient::_loadNextMedia()
{
	loadMediaRequest(lastMediaResponse.getPagination().getNextURL());
}

void InstagramClient::loadMediaRequest(const string& request)
{
	string json = Curl::get(request);

	lastMediaResponse.parse(json);

	if (loadImagesImmediately)
		loadImages();

	_loading = false;
}

void InstagramClient::loadStandartResImageByIndex(int index)
{
	lastHiResIndex = index;
	bool toLoad = false;

	cashed_mutex.lock();
	toLoad = (lastHiResIndex < cashedTextures.size());
	cashed_mutex.unlock();

	if (toLoad)
		mediaBigLoadThread = ThreadRef(new boost::thread(bind(&InstagramClient::_loadStandartResImageByIndex, this)));
}

void InstagramClient::_loadStandartResImageByIndex()
{
	if (kill)
		return;
	cashed_mutex.lock();
	ImageGraphic img = cashedTextures[lastHiResIndex];
	cashed_mutex.unlock();
	img.loadStandartRes();
	if(kill)
		return;
	cashed_mutex.lock();
	cashedTextures[lastHiResIndex] = img;
	cashed_mutex.unlock();
}

gl::Texture InstagramClient::getLastStandartResTexture()
{
	cashed_mutex.lock();
	ci::gl::Texture tex = cashedTextures[lastHiResIndex].getStandartResImage();
	cashed_mutex.unlock();
	return tex;
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

vector<ImageGraphic> InstagramClient::getImages()
{
	std::vector<ImageGraphic> images;
	cashed_mutex.lock();
	images = cashedTextures;
	cashed_mutex.unlock();
	return images;
}

list<User> InstagramClient::getUsers() const
{
	return userList;
}

void InstagramClient::loadImages()
{
	std::vector<ImageGraphic> textures_temp;
	list<InstagramMedia> mediaList = lastMediaResponse.getData();

	for (auto image : mediaList)
	{
		ImageGraphic imageGr;
		imageGr.setLowResURL(image.getImagesList().getLowResolution().getURL());
		imageGr.setStandartResURL(image.getImagesList().getStandardResolution().getURL());
		imageGr.setSize(343);
		textures_temp.push_back(imageGr);
	}

	cashed_mutex.lock();
	for (auto imageGr : textures_temp)
		cashedTextures.push_back(imageGr);
	int startIndex = cashedTextures.size() - textures_temp.size();
	cashed_mutex.unlock();


	using namespace ci::app;

	for (size_t i = 0; i < textures_temp.size(); i++)
	{		
		if (kill) break;
		
		textures_temp[i].loadLowRes();
		if (kill) break;
		console() << "textures_temp size:::::::::::::::  " << textures_temp.size() << endl;
		console() << "cashedTextures size:::::::::::::::  " << cashedTextures.size() << endl;
		console() << "startIndex + i:::::::::::::::  " << startIndex + i << endl;
		cashed_mutex.lock();
		cashedTextures[startIndex + i] = textures_temp[i];
		cashed_mutex.unlock();
	}

	//cashed_mutex.lock();
	//for (size_t i = 0; i < textures_temp.size(); i++)
	//{
	//	//textures_temp[i].loadLowRes();		
	//	cashedTextures[startIndex + i] = textures_temp[i];		
	//}
	//cashed_mutex.unlock();
}

void InstagramClient::clear()
{
	cashed_mutex.lock();
	cashedTextures.clear();
	cashed_mutex.unlock();
}

void InstagramClient::killLoad()
{
	using namespace ci::app;
	console() << "TRY TO KILLED!!!!!!!!!!!!!!!" << endl;
	if (_loading)
	{
		kill = true;	
		if (mediaLoadThread && mediaLoadThread->joinable())
			mediaLoadThread->join();
		if (mediaBigLoadThread && mediaBigLoadThread->joinable())
			mediaBigLoadThread->join();
	}
	kill = false;
	console() << "KILLED!!!!!!!!!!!!!!!" << endl;
}