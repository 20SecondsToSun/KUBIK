#pragma once

#include "gui/Sprite.h"
#include "main/gamesBlock/ToolButton.h"
#include "settings/ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolField> ToolFieldRef;

		class ToolField: public Sprite
		{
		public:
			ToolField(ConfigSettingsRef config, const GamesInfo& info);
			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			virtual void draw() override;

			void drawOffMessage();
			void setActive(bool isActive);			
			void mouseUpFunction(EventGUIRef& event);
			void setAlpha(float alpha);
			void swapActive();

		private:
			bool isActive;
		
			ConfToolButtonRef toolBtn;
			StatToolButtonRef statBtn;

			TextItem offText;			
		};
	}
}