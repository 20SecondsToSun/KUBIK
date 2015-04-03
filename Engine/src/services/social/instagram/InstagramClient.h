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
	}

	class InstagramClient
	{
	public:
		InstagramClient(const std::string& clientID);

		////////////////////////////////////////////////////////////////////////////
		//
		//					LOAD BY TAG
		//
		////////////////////////////////////////////////////////////////////////////

		void loadTagMedia(const std::string& tagName, int count = 20);
		void _loadTagMedia(const std::string& tagName, int count = 20);

		////////////////////////////////////////////////////////////////////////////
		//
		//					PAGINATION
		//
		////////////////////////////////////////////////////////////////////////////

		void loadNextMedia();
		void _loadNextMedia();

		void loadMediaRequest(const std::string& request);
		void loadStandartResImageByIndex(int index);

		void _loadStandartResImageByIndex();
		ci::gl::Texture getLastStandartResTexture();

		////////////////////////////////////////////////////////////////////////////
		//
		//					GETTERS
		//
		////////////////////////////////////////////////////////////////////////////

		bool isLoading() const;
		std::vector<ImageGraphic> getImages();
		std::list<User> getUsers() const;

		void clear();
		void killLoad();		

	protected:
		std::string clientID;

		ThreadRef mediaLoadThread, mediaBigLoadThread;

		size_t lastHiResIndex;
		bool _loading, loadImagesImmediately;

		bool kill;
		boost::mutex kill_mutex;

		std::vector<ImageGraphic> cashedTextures;
		boost::mutex cashed_mutex;

		std::shared_ptr<ci::Surface> lastLoadedStandartResImage;
		boost::mutex standartImageMutex;

		std::list<User> userList;

		std::string nextRequest;
		InstagramResponse<InstagramMedia> lastMediaResponse;

		void loadImages();
	};
}