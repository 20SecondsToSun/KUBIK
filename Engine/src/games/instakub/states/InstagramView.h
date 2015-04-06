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
			typedef std::shared_ptr<class InstagramView> InstagramViewRef;

			class InstagramView : public Sprite
			{				
				float yPosition;

				InstagramClientRef instClient;
				InstagramViewerRef instaViewer;
				InstaPopupRef instaPopup;

				void touchedHandler();
				void closePopupHandler();
				void printPopupHandler();
				void disconnectPopup();

				void synchHandler();
				void startLoadHandler();

			public:
				InstagramView(const gl::Texture& close, const gl::Texture& save, const gl::Texture& _template);
				void draw();
				void drawPopup();
				void load();
				void start();				
				void stop();
				void clear();				
				void setPosition(float x, float y);
			};
		}
	}
}