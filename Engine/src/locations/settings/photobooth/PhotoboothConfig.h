#pragma once
#include "elements/IPhotoboothItem.h"
#include "elements/PhotoFilters.h"
#include "elements/PhotoOverElements.h"
#include "elements/PhotoCardStyle.h"
#include "elements/Sharing.h"
#include "OpenPhotoBoothLayoutEvent.h"
#include "GameSettingsSprite.h"

namespace kubik 
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoboothConfig> PhotoboothConfigRef;

		class PhotoboothConfig: public GameSettingsSprite
		{
		public:	
			PhotoboothConfig(PhotoboothSettingsRef phbSettings);
			
			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void showAnimate(const ci::EaseFn& eFunc, float time);
			virtual void hideAnimate(const ci::EaseFn& eFunc, float time);
		
			void mouseUpHandler(EventGUIRef& event);
			void setOpenItem(int index);			
			void animationPosUpdate();
			void showAnimationFinish();			
			void hideAnimationFinish();

		private:
			int leftMargin;
			int itemsCount;
			int oneItemCloseHeightMax, oneItemCloseHeightMin, oneItemOpenHeight;
			int openItemIndex;

			PhotoboothSettingsRef phbSettings;
			PhotoOverElementsRef  photoOverElements;
			PhotoCardStyleRef     photoCardStyles;
			PhotoFiltersRef	      photoFilters;
			SharingRef		      sharing;

			ci::Anim<ci::Vec2f>	  animatePosition;
		};
	}
}