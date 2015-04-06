#pragma once
#include "gui/Sprite.h"
#include "InstakubSettings.h"
#include "instagram/InstagramClient.h"
#include "instagram/InstagramViewer.h"
#include "instagram/InstaPopup.h"

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
				void initOverMask();
			
				void fillBg();
				virtual void drawTitle();
				void initPosition();			
				void hashTagOnlyload(const string& hashtag);
				void clear();
				void drawPopup();

			protected:
				InstakubSettingsRef settings;

			private:	
				static InstagramClientRef instClient;
				static InstagramViewerRef instaViewer;
				static InstaPopupRef instaPopup;

				float yPosition;
				ci::Vec2f position;

				void touchedHandler();
				void closePopupHandler();
				void printPopupHandler();
				void disconnectPopup();

				void synchHandler();
				void startLoadHandler();
				void noMoreHandler();				
			};
		}
	}
}