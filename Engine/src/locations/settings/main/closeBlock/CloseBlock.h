#pragma once

#include "gui/ImageButtonSprite.h"
#include "BackToMainConfigEvent.h"
#include "CloseConfigEvent.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class CloseBlock> CloseBlockRef;

		class CloseBlock: public ImageButtonSprite
		{
		public:	
			CloseBlock(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			void animateToMiniState(const ci::EaseFn& eFunc, float time, const ci::Vec2i& finPos);
			void animateToMaxState(const ci::EaseFn& eFunc, float time);

			private:
				ci::gl::Texture iconClose, iconBack;
				EventGUIRef closeEvent, backEvent;
		};	
	}
}