#pragma once
#include "gui/Sprite.h"

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
		class PhotoboothConfig: public Sprite
		{
		public:	
			PhotoboothConfig(PhotoboothSettingsRef phbSettings)
				:Sprite(), leftMargin(165)				
			{					
				sharing			  = SharingRef(new Sharing(phbSettings, Color::hex(0x8e47aa), 0));
				photoOverElements = PhotoOverElementsRef(new PhotoOverElements(phbSettings,	Color::hex(0x01a7fb), 1));
				photoCardStyles   = PhotoCardStylesRef(new PhotoCardStyles(phbSettings, Color::hex(0x3e82df), 3));
				photoPrintCount   = PhotoPrintCountRef(new PhotoPrintCount(phbSettings, Color::hex(0x5e6fd1), 2));
				photoFilters	  = PhotoFiltersRef(new PhotoFilters(phbSettings, Color::hex(0x7e5cc2), 4));

				addChild(sharing);
				addChild(photoPrintCount);
				addChild(photoOverElements);
				addChild(photoCardStyles);				
				addChild(photoFilters);		
			}		

			void activateListeners()
			{
				for (auto layout : displayList)	
				{
					layout->connectEventHandler(&PhotoboothConfig::mouseUpHandler, this);
					layout->activateListeners();
				}		

				//fake
				//setOpenItem(0);
			}

			void unActivateListeners()
			{
				for (auto layout : displayList)	
				{
					layout->disconnectEventHandler();
					layout->unActivateListeners();
				}
			}

			void mouseUpHandler(EventGUIRef& _event)
			{
				EventGUI *ev = _event.get();
				if(!ev)
					return;

				console()<<":::::::::::::::clicked::::::::::::::::::::"<<endl;

				if(typeid(*ev) == typeid(OpenPhotoBoothLayoutEvent))
				{					
					OpenPhotoBoothLayoutEventRef event = static_pointer_cast<OpenPhotoBoothLayoutEvent>(_event);	
					setOpenItem(event->getlayoutIndex());
				}
				else if(typeid(*ev) == typeid(CheckerSocialEvent))
				{	
					CheckerSocialEventRef event = static_pointer_cast<CheckerSocialEvent>(_event);	
					console()<<"CheckerSocialEvent:::: "<<event->getSocialID()<<" ....  "<<event->getValue()<<endl;
				}
				else if(typeid(*ev) == typeid(PhotoTemplateChooseEvent))
				{
					PhotoTemplateChooseEventRef event = static_pointer_cast<PhotoTemplateChooseEvent>(_event);	
					console()<<"PhotoTemplateChooseEvent:::: "<<event->getCount()<<endl;
				}		
			}

			void setOpenItem(int index)
			{	
				for (auto layout : displayList)		
				{
					if (index == (static_pointer_cast<IPhotoboothItem>(layout))->getIndex())
						(static_pointer_cast<IPhotoboothItem>(layout))->activateListeners();	
					else
						(static_pointer_cast<IPhotoboothItem>(layout))->unActivateListeners();
			
					(static_pointer_cast<IPhotoboothItem>(layout))->setOpenLayoutIndex(index);	
				}				
			}	

			void showAnimate(EaseFn eFunc, float time)
			{	
				animatePosition = getGlobalPosition();
				unActivateListeners();
				ci::Vec2f finPos = ci::Vec2f(166, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn(bind( &PhotoboothConfig::showAnimationFinish, this))
						.updateFn(bind( &PhotoboothConfig::animationPosUpdate, this));
			}

			void animationPosUpdate()
			{
				setPosition(animatePosition.value());					
			}

			void showAnimationFinish()
			{
				activateListeners();
			}

			void hideAnimate(ci::EaseFn eFunc, float time)
			{
				unActivateListeners();
				ci::Vec2f finPos = ci::Vec2f(1080, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn(bind( &PhotoboothConfig::hideAnimationFinish, this))
						.updateFn(bind( &PhotoboothConfig::animationPosUpdate, this));
			}		

			void hideAnimationFinish()
			{		
				setOpenItem(-1);
			}			

		private:
			int leftMargin;
			int itemsCount;
			int oneItemCloseHeightMax, oneItemCloseHeightMin, oneItemOpenHeight;
			int openItemIndex;

			PhotoOverElementsRef photoOverElements;
			PhotoCardStylesRef   photoCardStyles;
			PhotoPrintCountRef   photoPrintCount;
			PhotoFiltersRef	     photoFilters;
			SharingRef		     sharing;

			ci::Anim<ci::Vec2f> animatePosition;
		};

		typedef std::shared_ptr<PhotoboothConfig> PhotoboothConfigRef;
	}
}