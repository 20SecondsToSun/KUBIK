#pragma once
#include "gui/Sprite.h"
#include "gui/ImageButtonSprite.h"
#include "PrinterControls.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PrinterBlock> PrinterBlockRef;

		class PrinterBlock: public Sprite
		{
		public:	
			static const int OPEN_EVENT = 0;
			static const int HIDE_EVENT = 1;
			static const int STAT_RESET_EVENT = 2;
			static const int OPENED = 3;
			static const int HIDED = 4;

			PrinterBlock(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			virtual void drawLayout() override;
			virtual void draw() override;
			virtual void eventListener(EventGUIRef event);

			void openButtonHandler(EventGUIRef& event);
			void openControls(ci::EaseFn eFunc = EaseOutCubic(), float time = 0.9f);
			void closeControls(ci::EaseFn eFunc = EaseOutCubic(), float time = 0.9f);
			void posAnimationUpdate();
			void hideControlsAnimationFinish();
			void openControlsAnimationFinish();	

			void setAlpha(float alpha);			
			void setMaxPhotosToPrint(int value);
			void set—urrentPhotosPrinted(int value);
		
		private:
			ci::Vec2i position, digitTexturePos, hintTextTexturePos;
			ci::gl::Texture icon, digitTexture, hintTextTexture;			
			ci::ColorA numsColor, bckgrndColor, barColor2, barColor1, iconColor;
			ci::Font numsFont;
			ci::Rectf btnRectf;
			ci::Anim<ci::Vec2f> animatePosition;

			TextItem hintText, changeBtnText;
			ImageButtonSpriteRef openBtn;
			PrinterControlsRef controls;

			int maxPhotosToPrint, currentPhotosPrinted, curBarWidth;			
			float maxBarWidth;							
		};		
	}
}