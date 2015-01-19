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
				:leftMargin(165),
				itemsCount(6),
				oneItemCloseHeightMax(320),
				oneItemCloseHeightMin(171),
				oneItemOpenHeight(1065)
			{
				openItemIndex = -1;	
				int index = 0;

				Color colors [] = { Color::hex(0x01a7fb), Color::hex(0x1f95ed), Color::hex(0x3e82df),
									Color::hex(0x5e6fd1), Color::hex(0x7e5cc2), Color::hex(0x8e47aa)};

				string mainTitles[] = { "Дизайн интерфейса", "Элементы поверх фото", 
										"Количество фото на печать", "Элементы поверх фото",
										"Фотофильтры", "Публикация фото" };
				string subTitles[]  = { "Minimal Epileptic", "Индивидуальный",
										"Gift Boxes", "3 фотографии",
										"Hudson, Amaretto, White&Black, Sepia", "Печать, Email, QR-code, Twitter" };

				interfaceDesign = InterfaceDesignRef( new InterfaceDesign(configSettings, phbSettings, index++));				
				layouts.push_back(interfaceDesign);				
				
				photoOverElements = PhotoOverElementsRef( new PhotoOverElements(phbSettings, index++));				
				layouts.push_back(photoOverElements);

				photoCardStyles = PhotoCardStylesRef( new PhotoCardStyles(phbSettings, index++));
				layouts.push_back(photoCardStyles);

				photoPrintCount = PhotoPrintCountRef( new PhotoPrintCount(phbSettings, index++));
				layouts.push_back(photoPrintCount);

				int i = 0;

				for (auto layout : layouts)
				{
					layout->setParams( colors[i],
									   mainTitles[i],
									   subTitles[i],
									   configSettings->getFont("introLight44"),
									   configSettings->getFont("helveticaLight24"),
									   oneItemCloseHeightMin,
									   oneItemCloseHeightMax,
									   oneItemOpenHeight);			

					i++;
				}					
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
				interfaceDesign->setPosition(position);
				photoOverElements->setPosition(position);
				photoCardStyles->setPosition(position);
				photoPrintCount->setPosition(position);
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