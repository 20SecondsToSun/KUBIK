#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "photobooth/elements/PhotoCountTemplateButton.h"

namespace kubik
{
	namespace config
	{
		class PhotoPrintCount: public IPhotoboothItem
		{
		public:	
			PhotoPrintCount(PhotoboothSettingsRef settings, ci::Color color, int index)
				:IPhotoboothItem(settings, PhtTextID::PRINT_COUNT, color, index)
			{
				photoTemplate1 = settingsFactory()
					.createPhotoCountTemplateButton(
					settings->getTexture("photoTemplate1"),
					PhtTextID::PHOTO_TREMPLATE_1,
					settings->getPhotoCount(PhtTextID::PHOTO_TREMPLATE_1));

				photoTemplate2 = settingsFactory()
					.createPhotoCountTemplateButton(
					settings->getTexture("photoTemplate2"),
					PhtTextID::PHOTO_TREMPLATE_2, 
					settings->getPhotoCount(PhtTextID::PHOTO_TREMPLATE_2));

				photoTemplate1->setPosition(ci::Vec2f(148.0f, 388.0f));
				photoTemplate2->setPosition(ci::Vec2f(490.0f, 388.0f));				

				if (settings->getCurrentPhotoCount() == settings->getPhotoCount(PhtTextID::PHOTO_TREMPLATE_2))
					photoTemplate2->setSelection(true);
				else
					photoTemplate1->setSelection(true);

				addChild(photoTemplate1);
				addChild(photoTemplate2);
			}	

			virtual void activateListeners()
			{
				photoTemplate1->connectEventHandler(&PhotoPrintCount::checkerClicked1, this);
				photoTemplate2->connectEventHandler(&PhotoPrintCount::checkerClicked2, this);
				IPhotoboothItem::activateListeners();
			}

			virtual void mainTitleClicked(EventGUIRef& event)
			{
	
			}

			void checkerClicked1(EventGUIRef event)
			{
				if(!photoTemplate1->getSelection())
				{
					photoTemplate1->setSelection(true);
					photoTemplate2->setSelection(false);
				}
			}

			void checkerClicked2(EventGUIRef event)
			{
				if(!photoTemplate2->getSelection())
				{
					photoTemplate2->setSelection(true);
					photoTemplate1->setSelection(false);
				}
			}

			void unActivateListeners()
			{
				photoTemplate1->disconnectEventHandler();
				photoTemplate2->disconnectEventHandler();
				IPhotoboothItem::unActivateListeners();
			}

			virtual void drawLayout()
			{
				gl::color(Color::white());			
			}

		private:
			PhotoCountTemplateButtonRef photoTemplate1, photoTemplate2;
		};

		typedef std::shared_ptr<PhotoPrintCount> PhotoPrintCountRef;
	}
}