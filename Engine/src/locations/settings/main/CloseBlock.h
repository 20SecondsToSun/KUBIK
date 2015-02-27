#pragma once
#include "stdafx.h"

#include "gui/ImageButtonSprite.h"
#include "BackToMainConfigEvent.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock: public ImageButtonSprite
		{
		public:	
			CloseBlock(ConfigSettingsRef configSettings, ci::Vec2i position)
				:ImageButtonSprite(configSettings->getTexture("iconClose"), position),
				iconClose(configSettings->getTexture("iconClose")),
				iconBack(configSettings->getTexture("iconBack"))
			{
				event = closeEvent = CloseConfigEventRef(new CloseConfigEvent());	
				backEvent = BackToMainConfigEventRef(new BackToMainConfigEvent());
				setPosition(position);
			}	

			void animateToMiniState(const ci::EaseFn& eFunc, float time, Vec2f finPos)
			{			
				setPosition(_localPosition - ci::Vec2f(40.0f, 0.0f));
				event = backEvent;

				changeTexture(iconBack);
			}

			void animateToMaxState(const ci::EaseFn& eFunc, float time)
			{
				setPosition(_localPosition + ci::Vec2f(40.0f, 0.0f));
				event = closeEvent;
				changeTexture(iconClose);
			}

			private:
				Texture iconClose, iconBack;
				EventGUIRef closeEvent, backEvent;
		};

		typedef std::shared_ptr<CloseBlock> CloseBlockRef;
	}
}