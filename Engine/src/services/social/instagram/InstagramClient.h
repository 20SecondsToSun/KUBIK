#pragma once

#include "Curl.h"
#include "instagram/api/ImagesList.h"
#include "instagram/api/Pagination.h"
#include "instagram/api/Tag.h"
#include "instagram/api/User.h"
#include "instagram/api/InstagramResponse.h"
#include "instagram/api/InstagramMedia.h"
#include "instagram/api/InstBaseObject.h"
#include "instagram/api/MetaData.h"
#include "instagram/ImageGraphic.h"
#include "Utils.h"

namespace instagram
{
	typedef boost::shared_ptr<boost::thread> ThreadRef;
	typedef std::shared_ptr<class InstagramClient> InstagramClientRef;

	namespace API
	{
		const static std::string POPULAR = "https://api.instagram.com/v1/media/popular/";
		const static std::string MEDIA_ID = "https://api.instagram.com/v1/media/";
		const static std::string USER_SEARCH = "https://api.instagram.com/v1/users/search";
		const static std::string USERS = "https://api.instagram.com/v1/users/";
		const static std::string TAGS = "https://api.instagram.com/v1/tags/";
		const static std::string SEARCH = "search/";
		const static std::string MEDIA_RECENT = "media/recent/";
		const static std::string CLIENT_ID = "client_id";
		const static std::string QUERY = "q";

		const static int USER_NOT_EXIST = -1000;
	}

	class InstagramClient
	{
		static const int THUMB_SIZE = 342;
		static const int LOAD_COUNT = 18;

		typedef ci::signals::signal<void(void)> SignalVoid;

	public:
		InstagramClient(const std::string& clientID);
		std::string clientIDParam() const;

		////////////////////////////////////////////////////////////////////////////
		//
		//					LOAD POPULAR
		//
		////////////////////////////////////////////////////////////////////////////

		void loadPopular(int count = LOAD_COUNT);
		void _loadPopular(int count = LOAD_COUNT);

		////////////////////////////////////////////////////////////////////////////
		//
		//					LOAD BY TAG
		//
		////////////////////////////////////////////////////////////////////////////

		void loadTagMedia(const std::string& tagName, int count = LOAD_COUNT);
		void _loadTagMedia(const std::string& tagName, int count = LOAD_COUNT);

		////////////////////////////////////////////////////////////////////////////
		//
		//					USER LOAD
		//
		////////////////////////////////////////////////////////////////////////////

		void loadUserMedia(const std::string& tagName, int count = LOAD_COUNT);
		void _loadUserMedia(const std::string& tagName, int count = LOAD_COUNT);
		void loadUserPhotos(const std::string& userID, int count = LOAD_COUNT);
		void loadUsersRequest(const std::string& request);		
	
		////////////////////////////////////////////////////////////////////////////
		//
		//					PAGINATION
		//
		////////////////////////////////////////////////////////////////////////////

		void loadNextMedia();
		void _loadNextMedia();
		void loadMediaRequest(const std::string& request);		
	
		////////////////////////////////////////////////////////////////////////////
		//
		//					GETTERS
		//
		////////////////////////////////////////////////////////////////////////////

		bool isLoading() const;
		bool canLoad() const;
		bool needSynch() const;
		bool userPrivate() const; 
		bool userNotExist() const;
		int getLastCode() const;
		bool userNotHavePhotos() const;
		bool noHashtagPhotos() const;
		void setSynch(bool val);		

		void setupLoadThread();
		void update();

		std::vector<ImageGraphic> getImages() const;
	
		void killLoad();

		SignalVoid synchEvent, startLoadEvent, noMoreEvent;
		ci::signals::scoped_connection updateCon;

		InstagramResponse<InstagramMedia> getLastMediaResponse() { return lastMediaResponse; };

	protected:
		std::string clientID;
		bool _loading, _needSynch, _noMore;

		int lastCode;

		std::vector<ImageGraphic> synchImages;
		std::list<User> userList;

		std::string nextRequest;
		InstagramResponse<InstagramMedia> lastMediaResponse;
		InstagramResponse<User> userResponse;


		void loadImages();

		ThreadRef mediaLoadThread;
	};
}