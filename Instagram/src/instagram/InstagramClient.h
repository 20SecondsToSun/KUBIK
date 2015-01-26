#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "InstagramClientInterface.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

using namespace std;
using namespace mndl::curl;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	namespace instagram
	{
		class InstagramClient : public InstagramClientInterface
		{

		public:	
			InstagramClient(string clientID):InstagramClientInterface(clientID)
			{

			}

			void loadPopularMedias()
			{		
				string request = API::POPULAR + "?" + API::CLIENT_ID + "=" + clientID;
				console()<<"request-------:  "<<request<<endl;

				string json =  Curl::get(request);
				JsonTree data = JsonTree(json).getChild("data");				
				
				medialist.clear();

				for(auto it : data)				
					medialist.push_back(getOneMedia(it));				

				loadImages();
			}

			void userSearch(string name)
			{
				string request = API::USER_SEARCH + "?" + API::QUERY + "=" + name + "&" + API::CLIENT_ID + "=" + clientID;
				console()<<"request-------:  "<<request<<endl;
				string json =  Curl::get(request);
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
					medialist.push_back(getOneMedia(it));	

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

			void loadTagMedia(string tagName)
			{	
				ThreadRef( new boost::thread( bind( &InstagramClient::_loadTagMedia, this, tagName) ) );				
			}

			void _loadTagMedia(string tagName)
			{
				string request = API::TAGS + tagName + "/" + API::MEDIA_RECENT + "?" + API::CLIENT_ID + "=" + clientID;
				console()<<"request-------:  "<<request<<endl;
				string json =  Curl::get(request);
				JsonTree data  = JsonTree(json).getChild("data");
				console()<<"----------------:  "<<data<<endl;
				JsonTree pagination  = JsonTree(json).getChild("pagination");					
				
				console()<<"pagination-------:  "<<pagination<<endl;	

				medialist.clear();
				for(auto it : data)				
					medialist.push_back(getOneMedia(it));	

				console()<<"medialist-------:  "<<medialist.size()<<endl;	

				loadImages();
			}

			Media getOneMedia(JsonTree tree)
			{
				string mediaID = tree.getChild("id").getValue<string>();		
				string link = tree.getChild("link").getValue<string>();		
				JsonTree images = tree.getChild("images");	
					
				Media media;
				media.setLowResImageData(parseImageData(images.getChild("low_resolution")));
				media.setThumbData(parseImageData(images.getChild("thumbnail")));
				media.setStandartData(parseImageData(images.getChild("standard_resolution")));

				media.setID(mediaID);
				media.setLink(link);

				return media;
			}

			Media loadOneMedia(string id)
			{				
				string request = API::MEDIA_ID  + id + "?" + API::CLIENT_ID + "=" + clientID;
				string json    = Curl::get(request);
				JsonTree data  = JsonTree(json).getChild("data");	

				string link = data.getChild("link").getValue<string>();	

				JsonTree images = data.getChild("images");	

				Media media;
				media.setLowResImageData(parseImageData(images.getChild("low_resolution")));
				media.setThumbData(parseImageData(images.getChild("thumbnail")));
				media.setStandartData(parseImageData(images.getChild("standard_resolution")));

				media.setID(id);
				media.setLink(link);

				return media;
			}

			list<Surface> getImages()
			{
				return textures;
			}

			UserList getUsers()
			{
				return userList;
			}
	
		protected:
			UserList userList;
			MediaList medialist;
			list<Surface> textures;

			ImageData parseImageData(JsonTree json)
			{
				ImageData imgdata;

				string url = json.getChild("url").getValue<string>();	
				int width  = json.getChild("width").getValue<int>();	
				int height = json.getChild("height").getValue<int>();

				imgdata.setWidth(width);
				imgdata.setHeight(height);
				imgdata.setURL(url);
				return imgdata;
			}	

			void loadImages()
			{
				textures.clear();
				for (auto media : medialist)
				{
					textures.push_back(loadImage( loadUrl(media.getLowResImageData().getURL())));
				}
			}
		};

		typedef shared_ptr<InstagramClient> InstagramClientRef;
	}
}