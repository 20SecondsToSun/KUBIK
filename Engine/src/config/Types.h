#pragma once

#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
//using namespace kubik::config;

namespace kubik
{	

	class ApplicationView;
	//class ApplicationModel;
	class Controller;
	class IButton;

	typedef shared_ptr<ApplicationView> AppViewRef;
	//typedef shared_ptr<ApplicationModel> AppModelRef;
	typedef shared_ptr<Controller> ControllerRef;

	typedef boost::shared_ptr<boost::thread> ThreadRef;

	typedef ci::signals::signal<void(void)> SignalVoid;
	typedef ci::signals::signal<void(KubikException)> SignalException;
	typedef boost::signals2::signal<void(IButton&)> ButtonSignal;
	enum DesignType
	{
		KUBIK,
		CUSTOM
	};

	namespace game
	{
		enum id
		{
			UNDEFINED = 0,
			FUNCES = 1,
			PHOTOBOOTH = 2,
			INSTAKUB = 3,
			KOTOPOZA = 4
		};
	}

	namespace changeSetting
	{	
		enum id
		{
			MENU = 10,
			CONFIG = 20,
			SCREENSAVER = 30,
			PRELOADER = 40,
			SERVICEPOPUP = 50,
			GAMES = 60,
			FUNCES = game::FUNCES,
			PHOTOBOOTH = game::PHOTOBOOTH,
			KOTOPOZA = game::KOTOPOZA,
			INSTAKUB = game::INSTAKUB
		};	
	}

	namespace screen
	{	
		enum id
		{
			UNDEFINED = 0,
			MENU = 10,
			CONFIG = 20,
			SCREENSAVER = 30,
			PRELOADER = 40,
			SERVICEPOPUP = 50,
			GAMES = 60,
			FUNCES = game::FUNCES,
			PHOTOBOOTH = game::PHOTOBOOTH,
			KOTOPOZA = game::KOTOPOZA,
			INSTAKUB = game::INSTAKUB
		};	
	}

	namespace settings
	{
		enum id
		{
			PHOTOBOOTH,
			INSTAKUB,
			FUNCES,
			MAINCONFIG,
			GAMES,
			MENU,
			SCREENSAVER
		};
	}

	struct IconPair
	{
		IconPair(Texture activeIcon, Texture unActiveIcon):activeIcon(activeIcon),unActiveIcon(unActiveIcon){}
		IconPair(){}
		Texture activeIcon;
		Texture unActiveIcon;
	};

	struct GamesInfo
	{
		game::id id;
		bool isOn, isPurchased;
		string name;
		IconPair iconPair;
		Texture miniIcon, texture;

		bool isGameOn() const
		{
			return isOn;
		}

		game::id getGameId() const
		{
			return id;
		}

		string getNameText() const
		{
			return name;
		}

		IconPair getIcons() const
		{
			return iconPair;
		}

		void setActiveIcon(Texture tex)
		{
			iconPair.activeIcon = tex;		
		}

		void setUnActiveIcon(Texture tex)
		{
			iconPair.unActiveIcon = tex;
		}

		void setMiniIcon(Texture tex) 
		{
			miniIcon = tex;
		}

		Texture getMiniIcon() const
		{
			return miniIcon;
		}

		void setTexture(const ci::gl::Texture& tex)
		{
			texture = tex;
		}

		Texture getTexture() const
		{
			return texture;
		}
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
			resourceType = resourceType::IMAGE;
		}
	};

	class FontResource: public IResource<Font>
	{
	public:	
		FontResource()
		{
			resourceType = resourceType::FONT;
		}			
		float fontSize;
	};

	class VideoResource: public IResource<qtime::MovieGl>
	{
	public:	
		VideoResource()
		{
			resourceType = resourceType::VIDEO;
		}
	};
	typedef shared_ptr<ImageResource> ImageResourceRef;
	typedef shared_ptr<FontResource>  FontResourceRef;
	typedef shared_ptr<VideoResource> VideoResourceRef;
	typedef shared_ptr<IResourceBase> IResourceBaseRef;

	typedef map<string, ImageResourceRef> ImageResourceDictionary;
	typedef map<string, FontResourceRef>  FontResourceDictionary;
	typedef map<string, VideoResourceRef> VideoResourceDictionary;
	typedef map<string, IResourceBaseRef> IResourceDictionary;

	typedef screen::id ScreenId;
	typedef game::id GameId;
	typedef changeSetting::id ChangeId;

	typedef struct _changes
	{
		changeSetting::id id;
		bool texReload;
		bool gamesReload;

		_changes()
		{
			texReload = false;
			gamesReload = false;
		}
	}
	Changes;

	template<typename F, typename S>
	struct Pair
	{
		Pair(F param1, S param2):param1(param1),param2(param2){}
		F param1;
		S param2;
	};

	template<typename F, typename S>
	struct Line
	{
		Line(){};
		F point1;
		S point2;
	};
}

template <typename Sig, typename F> void connect_once(Sig& sig, F&& f) 
{		
	if (!sig.num_slots()) 
		sig.connect(std::forward<F>(f));
}