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
		typedef std::shared_ptr<class PhotoCardStyle> PhotoCardStyleRef;

		class PhotoCardStyle: public Sprite
		{

		public:			
			PhotoCardStyle(InstakubSettingsRef settings, const ci::Vec2i& position);

			virtual void drawLayout();
			virtual void activateListeners();
			virtual void unActivateListeners();

			void drawBackground();
			void drawTitles();
			void buttonClicked(EventGUIRef& event);				

		private:
			ci::gl::Texture titleTextTex, subTitleTextTex;	
			ci::Vec2f titleTextPos, subTitleTextPos;

			InstakubSettingsRef settings;				
			SixButtonsLayerRef sixBtnLayer;
		};
	}
}