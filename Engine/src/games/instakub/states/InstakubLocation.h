#pragma once
#include "gui/Sprite.h"
#include "InstakubSettings.h"
#include "instagram/InstagramClient.h"
#include "instagram/InstagramViewer.h"
#include "instagram/InstaPopup.h"
#include "VirtualKeyboard.h"

using namespace kubik::config;
using namespace instagram;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class InstakubLocation> InstakubLocationRef;

			class InstakubLocation : public Sprite
			{			
			public:
				static const int ENABLE_CONTROLS = 1;
				static const int DISABLE_CONTROLS = 2;
				static const int SHOW_CONTROLS = 3;
				static const int HIDE_CONTROLS = 4;

			protected:
				enum LoadMode
				{
					POPULAR_MODE,
					HASHTAG_MODE
				} mode;

				static ci::gl::Texture bg;
				ci::gl::Texture title, overMask;
				ci::Vec2f titlePosition, searchFieldPosition;

				static bool init;

			public:
				InstakubLocation(InstakubSettingsRef settings, const ci::Vec2f& position);
				virtual void start() = 0;
				virtual void draw();
				virtual void stop() = 0;
				virtual void load() = 0;
				virtual void reset();
				virtual void reload();	

				virtual void drawTitle();
				virtual void openPopupHandler();

				void initOverMask();			
				void fillBg();			
				void initPosition();			
				void hashTagOnlyload(const std::string& hashtag);
				void loadPopuplar();				
				void clear();
				void drawPopup();

			protected:
				InstakubSettingsRef settings;
				
				static InstaPopupRef instaPopup;
				static InstagramViewerRef instaViewer;

				virtual void loadingCompleteHandler();
				virtual void startLoadHandler(); 
				virtual	void noMoreLoadsHandler();
				virtual void closePopupHandler();				

			private:	
				static InstagramClientRef instClient;						

				float yPosition;
				ci::Vec2f position;
				
				void reloadHandler();
				void nextLoadHandler();
				
				void printPopupHandler();
				void disconnectPopup();									
			};
		}
	}
}