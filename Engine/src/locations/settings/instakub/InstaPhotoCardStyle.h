#pragma once

#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "InstakubSettings.h"
#include "SixButtonsLayer.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class InstaPhotoCardStyle> InstaPhotoCardStyleRef;
		typedef std::shared_ptr<class SixButtonsLayer<ChangePhotoCardStyleDesignEvent>> SixButtonsLayerInstaRef;

		class InstaPhotoCardStyle: public Sprite
		{

		public:			
			InstaPhotoCardStyle(InstakubSettingsRef settings, const ci::Vec2i& position);

			virtual void drawLayout();
			virtual void activateListeners();
			virtual void unActivateListeners();

			void drawBackground();
			void drawTitles();
			void buttonClicked(EventGUIRef& event);				

		private:
			ci::Vec2f titleTextPos, subTitleTextPos;

			InstakubSettingsRef settings;				
			SixButtonsLayerInstaRef sixBtnLayer;

			ci::Color backgroundColor;

			kubik::config::TextItem titleItem;
			kubik::config::TextItem subTitleItem;
		};
	}
}