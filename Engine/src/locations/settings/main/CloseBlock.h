#pragma once
#include "stdafx.h"

#include "gui/ImageButton.h"
#include "BackToMainConfigEvent.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock: public ImageButton
		{
		public:	
			CloseBlock(ConfigSettingsRef configSettings, Vec2i position)
				:ImageButton(configSettings->getTexture("iconClose"), position),
				iconClose(configSettings->getTexture("iconClose")),
				iconBack(configSettings->getTexture("iconBack"))
			{
				event = closeEvent = CloseConfigEventRef(new CloseConfigEvent());	
				backEvent = BackToMainConfigEventRef(new BackToMainConfigEvent());
				setPosition(position);
			}	

			void animateToMiniState(EaseFn eFunc, float time, Vec2f finPos)
			{			
				setPosition(_localPosition - Vec2f(40,0));
				event = backEvent;

				changeTexture(iconBack);
			}

			void animateToMaxState(EaseFn eFunc, float time)
			{
				setPosition(_localPosition + Vec2f(40,0));
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