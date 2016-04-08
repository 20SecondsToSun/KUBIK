#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "HashChecker.h"
#include "InstakubSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class SearchBlock> SearchBlockRef;

		class SearchBlock: public Sprite
		{
		public:			
			SearchBlock(InstakubSettingsRef settings, const ci::Vec2i& position);

			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void drawLayout();

			void checkerClicked(EventGUIRef& event);
			bool isChecked();
			
		private:
			InstakubSettingsRef settings;	
			gl::Texture titleTextTex, subTitleTextTex;	
			ci::Vec2f titleTextPos, subTitleTextPos;
			HashCheckerRef checker;

			ci::Color backgroundColor;

			kubik::config::TextItem titleItem;
			kubik::config::TextItem subTitleItem;
		};
	}
}