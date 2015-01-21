#pragma once
#include "IDispatcher.h"
#include "IDrawable.h"

#include "elements/IPhotoboothItem.h"
#include "elements/PhotoFilters.h"
#include "elements/PhotoOverElements.h"
#include "elements/PhotoCardStyles.h"
#include "elements/PhotoPrintCount.h"
#include "elements/Sharing.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik 
{
	namespace config
	{
		class PhotoboothConfig: public IDispatcher
		{
		public:	
			PhotoboothConfig(PhotoboothSettingsRef phbSettings)
				:leftMargin(165)				
			{				
				int index = 0;		
				typedef PhotoboothSettings::SettingsPartID SettingsPartID;

				sharing = SharingRef(new Sharing(phbSettings, Color::hex(0x8e47aa), index++));
				layouts.push_back(sharing);

				photoOverElements = PhotoOverElementsRef(new PhotoOverElements(phbSettings,	Color::hex(0x01a7fb), index++));				
				layouts.push_back(photoOverElements);

				photoCardStyles = PhotoCardStylesRef(new PhotoCardStyles(phbSettings, Color::hex(0x3e82df), index++));
				layouts.push_back(photoCardStyles);

				photoPrintCount = PhotoPrintCountRef(new PhotoPrintCount(phbSettings, Color::hex(0x5e6fd1), index++));
				layouts.push_back(photoPrintCount);

				photoFilters = PhotoFiltersRef(new PhotoFilters(phbSettings, Color::hex(0x7e5cc2), index++));
				layouts.push_back(photoFilters);
			}		

			void activateListeners()
			{
				for (auto layout : layouts)
				{
					layout->activateListeners();
					layout->addMouseUpListener(&PhotoboothConfig::mouseUpHandler, this);
				}

				//fake
				setOpenItem(0);
			}

			void unActivateListeners()
			{
				for (auto layout : layouts)
				{
					layout->unActivateListeners();
					layout->removeMouseUpListener();
				}
			}

			void mouseUpHandler(EventRef& _event)
			{
				Event *ev = _event.get();

				if(typeid(*ev) == typeid(OpenPhotoBoothLayoutEvent))
				{
					OpenPhotoBoothLayoutEventRef event = static_pointer_cast<OpenPhotoBoothLayoutEvent>(_event);	
					setOpenItem(event->getlayoutIndex());
				}
				else if(typeid(*ev) == typeid(SavePhotobootnConfigEvent))
				{
					SavePhotobootnConfigEventRef event = static_pointer_cast<SavePhotobootnConfigEvent>(_event);	
					setOpenItem(-1);
				}				
			}

			void setOpenItem(int index)
			{
				for (auto layout : layouts)
				{
					layout->removeMainButtonListener();
					layout->setOpenLayoutIndex(index);
				}
			}

			virtual void draw()
			{				
				for (auto layout : layouts)
					layout->draw();			
			}

			void setInitPosition(Vec2i position)
			{
				animatePosition = position;
				setPosition(position);
			}	

			void showAnimate(EaseFn eFunc, float time)
			{	
				unActivateListeners();
				Vec2f finPos = Vec2f(166, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn( bind( &PhotoboothConfig::hideAnimationFinish, this, finPos, true))
						.updateFn(bind( &PhotoboothConfig::hideAnimationUpdate, this));
			}			

			void hideAnimate(EaseFn eFunc, float time)
			{
				unActivateListeners();
				Vec2f finPos = Vec2f(1080, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn( bind( &PhotoboothConfig::hideAnimationFinish, this, finPos, false))
						.updateFn(bind( &PhotoboothConfig::hideAnimationUpdate, this));
			}

			void hideAnimationUpdate()
			{
				position = Vec2f(animatePosition.value().x, position.y);
				for (auto layout : layouts)
					layout->animPosition(position);							
			}

			void hideAnimationFinish(Vec2f finPos, bool showAnim)
			{
				setPosition(finPos);	
				if(showAnim)
					activateListeners();
				else
					setOpenItem(-1);
			}

			void setPosition(Vec2f position)
			{
				IDispatcher::setPosition(position);
				for (auto layout : layouts)
					layout->setPosition(position);	
			}

		private:
			int leftMargin;
			int itemsCount;
			int oneItemCloseHeightMax, oneItemCloseHeightMin, oneItemOpenHeight;
			int openItemIndex;

			list<IPhotoboothItemRef> layouts;
			PhotoOverElementsRef photoOverElements;
			PhotoCardStylesRef   photoCardStyles;
			PhotoPrintCountRef   photoPrintCount;
			PhotoFiltersRef	     photoFilters;
			SharingRef		     sharing;

			Anim<Vec2f> animatePosition;
		};

		typedef std::shared_ptr<PhotoboothConfig> PhotoboothConfigRef;
	}
}