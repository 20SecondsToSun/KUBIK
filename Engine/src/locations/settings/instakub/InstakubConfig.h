#pragma once
#include "GameSettingsSprite.h"
#include "InstakubSettings.h"
#include "SearchBlock.h"
#include "HashTagBlock.h"
#include "InstaPhotoCardStyle.h"
#include "VirtualKeyboard.h"

namespace kubik 
{
	namespace config
	{
		typedef std::shared_ptr<class InstakubConfig> InstakubConfigRef;

		class InstakubConfig: public GameSettingsSprite
		{
		public:	
			InstakubConfig(InstakubSettingsRef instSettings);
			
			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void draw();
			virtual void showAnimate(const ci::EaseFn& eFunc, float time);
			virtual void hideAnimate(const ci::EaseFn& eFunc, float time);
		
			void mouseUpHandler(EventGUIRef& event);			
			void animationPosUpdate();
			void showAnimationFinish();			
			void hideAnimationFinish();
			void initKeyBoard();
			void inputTouchHandler();			

		private:					
			ci::Anim<ci::Vec2f> animatePosition;
			SearchBlockRef searchBlock;
			HashTagBlockRef hashTagBlock;
			InstaPhotoCardStyleRef photoCardStyle;
			InstakubSettingsRef instSettings;
		};		
	}
}