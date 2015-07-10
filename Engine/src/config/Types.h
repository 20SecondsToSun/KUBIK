#pragma once
#include "KubikException.h"

namespace kubik
{	
	class ApplicationView;
	class Controller;
	class IButton;

	typedef std::shared_ptr<ApplicationView> AppViewRef;
	typedef std::shared_ptr<Controller> ControllerRef;
	typedef std::shared_ptr<boost::thread> ThreadRef;

	typedef boost::signals2::signal<void(void)> SignalVoid;
	typedef boost::signals2::signal<void(KubikException)> SignalException;
	typedef boost::signals2::signal<void(IButton&)> ButtonSignal;

	typedef std::shared_ptr<class ImageResource> ImageResourceRef;
	typedef std::shared_ptr<class FontResource>  FontResourceRef;
	typedef std::shared_ptr<class VideoResource> VideoResourceRef;
	typedef std::shared_ptr<class IResourceBase> IResourceBaseRef;

	typedef std::map<std::string, ImageResourceRef> ImageResourceDictionary;
	typedef std::map<std::string, FontResourceRef>  FontResourceDictionary;
	typedef std::map<std::string, VideoResourceRef> VideoResourceDictionary;
	typedef std::map<std::string, IResourceBaseRef> IResourceDictionary;

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
			POZA = 4
		};
	}

	namespace changeSetting
	{	
		enum id
		{
			KEYBOARD = 0,
			MENU = 10,
			CONFIG = 20,
			SCREENSAVER = 30,
			PRELOADER = 40,
			SERVICEPOPUP = 50,
			GAMES = 60,
			SOCIAL = 70,
			FUNCES = game::FUNCES,
			PHOTOBOOTH = game::PHOTOBOOTH,
			POZA = game::POZA,
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
			KOTOPOZA = game::POZA,
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
			POZA,
			MAINCONFIG,
			GAMES,
			MENU,
			SCREENSAVER,
			PRELOADERS
		};
	}

	struct IconPair
	{
		IconPair(const ci::gl::Texture& activeIcon, const ci::gl::Texture& unActiveIcon):activeIcon(activeIcon), unActiveIcon(unActiveIcon){}
		IconPair(){}
		ci::gl::Texture activeIcon;
		ci::gl::Texture unActiveIcon;
	};

	struct GamesInfo
	{
		bool isOn, isPurchased;
		IconPair iconPair;
		game::id id;		
		std::string name;		
		ci::gl::Texture miniIcon, texture;

		bool isGameOn() const
		{
			return isOn;
		}

		game::id getGameId() const
		{
			return id;
		}

		std::string getNameText() const
		{
			return name;
		}

		IconPair getIcons() const
		{
			return iconPair;
		}

		void setActiveIcon(const ci::gl::Texture& tex)
		{
			iconPair.activeIcon = tex;		
		}

		void setUnActiveIcon(const ci::gl::Texture& tex)
		{
			iconPair.unActiveIcon = tex;
		}

		void setMiniIcon(const ci::gl::Texture& tex)
		{
			miniIcon = tex;
		}

		ci::gl::Texture getMiniIcon() const
		{
			return miniIcon;
		}

		void setTexture(const ci::gl::Texture& tex)
		{
			texture = tex;
		}

		ci::gl::Texture getTexture() const
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

		std::string path;
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

	class ImageResource: public IResource<ci::Surface>
	{
	public:	
		ImageResource()
		{
			resourceType = resourceType::IMAGE;
		}
	};

	class FontResource: public IResource<ci::Font>
	{
	public:	
		FontResource()
		{
			resourceType = resourceType::FONT;
		}			
		float fontSize;
	};

	class VideoResource: public IResource<ci::qtime::MovieGl>
	{
	public:	
		VideoResource()
		{
			resourceType = resourceType::VIDEO;
		}
	};	

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