#pragma once
#include "IDispatcher.h"
#include "IDrawable.h"

#include "elements/IPhotoboothItem.h"
#include "elements/InterfaceDesign.h"
#include "elements/PhotoOverElements.h"
#include "elements/PhotoCardStyles.h"
#include "elements/PhotoPrintCount.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik 
{
	namespace config
	{
		class PhotoboothConfig: public IDispatcher
		{
		public:	
			PhotoboothConfig(ConfigSettingsRef configSettings, PhotoboothSettingsRef phbSettings)
				:leftMargin(165)				
			{				
				int index = 0;

				//Color colors [] = { Color::hex(0x01a7fb), Color::hex(0x1f95ed), Color::hex(0x3e82df),
				//					Color::hex(0x5e6fd1), Color::hex(0x7e5cc2), Color::hex(0x8e47aa)};

				interfaceDesign = InterfaceDesignRef(new InterfaceDesign(configSettings, phbSettings, index++));				
				layouts.push_back(interfaceDesign);				
				
				photoOverElements = PhotoOverElementsRef(new PhotoOverElements(configSettings, phbSettings, index++));				
				layouts.push_back(photoOverElements);

				photoCardStyles = PhotoCardStylesRef(new PhotoCardStyles(configSettings, phbSettings, index++));
				layouts.push_back(photoCardStyles);

				photoPrintCount = PhotoPrintCountRef(new PhotoPrintCount(configSettings, phbSettings, index++));
				layouts.push_back(photoPrintCount);
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

			void setInitPosition(Vec2i position = Vec2i(166, 0))//1080, 0))
			{
				setPosition(position);
				for (auto layout : layouts)
					layout->setPosition(position);			
			}	

		private:
			int leftMargin;
			int itemsCount;
			int oneItemCloseHeightMax, oneItemCloseHeightMin, oneItemOpenHeight;
			int openItemIndex;

			list<IPhotoboothItemRef> layouts;
			InterfaceDesignRef interfaceDesign;
			PhotoOverElementsRef photoOverElements;
			PhotoCardStylesRef photoCardStyles;
			PhotoPrintCountRef photoPrintCount;
		};

		typedef std::shared_ptr<PhotoboothConfig> PhotoboothConfigRef;
	}
}