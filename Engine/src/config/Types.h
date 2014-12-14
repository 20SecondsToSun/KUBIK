#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/qtime/QuickTime.h"
#include <stdint.h>

using namespace std;
using namespace ci;
using namespace ci::gl;


namespace kubik
{
	struct GamesInfo
	{
		int id;
		bool isOn, isPurchased;
		string name;
	};

	enum LocationID
	{
		MENU = 10,
		CONTROL = 20,
		SCREENSAVER = 30,
		PRELOADER = 40,
		SERVICEPOPUP = 50,
	};

	enum gameId
	{
		FUNCES = 1,
		PHOTOBOOTH = 2,
		KOTOPOZA = 3
	};

	enum resourceType
	{
		IMAGE,
		VIDEO,
		FONT
	};

	enum loadingType
	{
		RESOURCE,
		ASSET,
		FULL_PATH,
		URL
	};

	class IResourceBase
	{
	public:	
		IResourceBase()
		{
			isLoading = false;
		}

		string path;
		bool isLoading;
		resourceType resourceType;
		loadingType  loadingType;
	};

	template <class T>
	class IResource: public IResourceBase
	{
	public:		
		T data;

		T get()
		{
			return data;
		}
		void set(T value)
		{
			data = value;
		}
	};

	class ImageResource: public IResource<Surface>
	{
	public:	
		ImageResource()
		{
			resourceType		= resourceType::IMAGE;
		}
	};

	class FontResource: public IResource<Font>
	{
	public:	
		FontResource()
		{
			resourceType		= resourceType::FONT;
		}			
		float fontSize;
	};

	class VideoResource: public IResource<qtime::MovieGl>
	{
	public:	
		VideoResource()
		{
			resourceType		= resourceType::VIDEO;
		}
	};

	typedef map<string, shared_ptr<ImageResource>> ImageResourceDictionary;
	typedef map<string, shared_ptr<FontResource>>  FontResourceDictionary;
	typedef map<string, shared_ptr<VideoResource>> VideoResourceDictionary;
	typedef map<string, shared_ptr<IResourceBase>> IResourceDictionary;

	typedef struct _changes
	{
		int id;
		bool texReload;
	}
	Changes;
}

template <typename Sig, typename F> void connect_once(Sig& sig, F&& f) 
{		
	if (!sig.num_slots()) 
		sig.connect(std::forward<F>(f));
}