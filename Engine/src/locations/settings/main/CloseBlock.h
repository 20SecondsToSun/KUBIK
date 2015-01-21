#pragma once
#include "stdafx.h"
#include "IDrawable.h"
#include "IDispatcher.h"
#include "Button.h"
#include "BackToMainConfigEvent.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock: public Button
		{
		public:	
			CloseBlock(ConfigSettingsRef configSettings, Vec2i position)
				:Button(configSettings->getTexture("iconClose"), position),
				iconClose(configSettings->getTexture("iconClose")),
				iconBack(configSettings->getTexture("iconBack"))
			{
				event = closeEvent = CloseConfigEventRef(new CloseConfigEvent());	
				backEvent = BackToMainConfigEventRef(new BackToMainConfigEvent());
				setPosition(position);
			}	

			void animateToMiniState(EaseFn eFunc, float time, Vec2f finPos)
			{			
				position -= Vec2f(40,0);
				Vec2f pos = position + finPos;
				setButtonArea(Rectf(pos, pos + Vec2f(getWidth(), getHeight())));
				event = backEvent;
				changeTexture(iconBack);
			}

			void animateToMaxState(EaseFn eFunc, float time)
			{
				position += Vec2f(40,0);
				
				setButtonArea(Rectf(position, position + Vec2f(getWidth(), getHeight())));
				event = closeEvent;
				changeTexture(iconClose);
			}

			private:
				Texture iconClose, iconBack;
				EventRef closeEvent, backEvent;
		};

		typedef std::shared_ptr<CloseBlock> CloseBlockRef;
	}
}