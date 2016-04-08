#pragma once

#include "gui/Sprite.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "photobooth/elements/MainTitleButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class IPhotoboothItem> IPhotoboothItemRef;

		class IPhotoboothItem: public Sprite
		{
		public:
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, const PhtTextID& id, const ci::Color& color, int index);

			virtual void activateListeners();
			virtual void activateTitleListeners();			
			virtual void unActivateListeners();
			virtual void mainTitleClicked(EventGUIRef& event);
			virtual void draw();
			virtual void drawContent();
			virtual void posAnimationUpdate();
			virtual void onOpenResetParams();

			void setSubTextTitle(const ci::gl::Texture&  tex);
			void setMainTextTitle(const ci::gl::Texture& tex);
			void setOpenLayoutIndex(int openLayoutIndex);;
			void closingLayoutMaxState(const ci::EaseFn& eFunc, float time);
			void closingLayoutMinState(const ci::EaseFn& eFunc, float time);
			void openingLayout(const ci::EaseFn& eFunc, float time);
			void animationFinish2();
			int getIndex() const;			

		protected:
			enum {CLOSE, OPEN, CLOSE_MIN} state;
			ci::Color color;
	
			ci::Font mainTextFont, subTextFont;
			int itemWidth, index, openLayoutIndex;
			int closeHeightMin, closeHeightMax, openHeight;
			
			ci::gl::Texture mainTextTex, subText;			

			ci::Anim<ci::Vec2f> animatePosition;
			ci::Anim<float> mainTitleY, subTitleY;
			ci::Anim<int> animHeight;

			float subTextTexPosX, mainTextTexPosX;

			float closeMaxY, closeMinY, openY;
			float offsetBetweenTitles;

			PhotoboothSettingsRef settings;
			MainTitleButtonRef mainTitleButton;	
			PhtTextID id;
		};		
	}
}