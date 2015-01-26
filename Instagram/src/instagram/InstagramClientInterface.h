#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include <boost/thread.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	namespace instagram
	{
		typedef boost::shared_ptr<boost::thread> ThreadRef;

		namespace API
		{
			const static string POPULAR		 = "https://api.instagram.com/v1/media/popular/";
			const static string MEDIA_ID	 = "https://api.instagram.com/v1/media/";
			const static string USER_SEARCH  = "https://api.instagram.com/v1/users/search";
			const static string USERS		 = "https://api.instagram.com/v1/users/";
			const static string TAGS		 = "https://api.instagram.com/v1/tags/";
			const static string SEARCH		 = "search/";			
			const static string MEDIA_RECENT = "media/recent/";			
			const static string CLIENT_ID	 = "client_id";
			const static string QUERY		 = "q";
		}
		
		class ImageData
		{
		public:
			void setWidth(int width)
			{
				this->width = width;
			}

			void setHeight(int height)
			{
				this->height = height;
			}

			void setURL(string url)
			{
				this->url = url;
			}

			string getURL()
			{
				return url;
			}

		private:
			int width;
			int height;
			string url;
		};

		class Media
		{
		public:
			void setID(string id)
			{
				this->id = id;
			}

			void setLink(string link)
			{
				this->link = link;
			}

			void setLowResImageData(ImageData imgdata)
			{
				lowResData = imgdata;
			}

			ImageData getLowResImageData()
			{
				return lowResData;
			}

			void setThumbData(ImageData imgdata)
			{
				thmbData = imgdata;
			}

			void setStandartData(ImageData imgdata)
			{
				standartResData = imgdata;
			}

		private:
			string id;
			string link;
			ImageData lowResData;
			ImageData thmbData;
			ImageData standartResData;
		};
		typedef list<Media> MediaList;

		class User
		{
		public:
			void setID(int id)
			{
				this->id = id;
			}

			int getID()
			{
				return id;
			}

		private:
			int  id;
		};

		typedef list<User> UserList;

		class InstagramClientInterface
		{

		public:	
			InstagramClientInterface(string clientID): clientID(clientID)
			{
			}

			virtual void  loadPopularMedias() = 0;					

		protected:	
			string clientID;
		};
	}
}