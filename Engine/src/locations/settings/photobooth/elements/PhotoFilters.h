#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "ChangePhotoFilterPreviewActiveEvent.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		class PhotoFilters: public IPhotoboothItem
		{
		public:	
			PhotoFilters(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, PhtTextID::FILTERS, color, index)
			{
				DesignData designdata = settings->getPhotoFiltersPreview();				

				ci::Vec2f pos = ci::Vec2f::zero();
				int i = 0;
				float shiftX = 53, shiftY = 130;
				float startX = 106, startY= 354;

				for (auto it : designdata)
				{				 
					Texture icon = settings->getTexture(it.getName());
					it.setIcon(icon);
					it.setFont(settings->getFonts());

					pos.x = startX + (icon.getWidth() + shiftX) * (i % 3);
					pos.y = startY + (icon.getWidth() + shiftY) * (i / 3);

					ImageQuadroButtonRef imageQuadroButton = settingsFactory().createPhotoFilterPreviewButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	
					i++;					
				}

				activeIDs = settings->getActiveFiltersIDs();	

				for (auto id : activeIDs)
					btns[id]->setSelection(true);				
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&PhotoFilters::buttonClicked, this);

				IPhotoboothItem::activateListeners();
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();	

				IPhotoboothItem::unActivateListeners();
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
				if(typeid(*ev) == typeid(ChangePhotoFilterPreviewActiveEvent))
				{		
					ChangePhotoFilterPreviewActiveEventRef statEvent = static_pointer_cast<ChangePhotoFilterPreviewActiveEvent>(event);	
					int id = statEvent->getItem().getID();
					btns[id]->swapSelection();					
				}
			}	

		private:
			std::map<int, ImageQuadroButtonRef> btns;
			std::vector<int> activeIDs; 
		};

		typedef std::shared_ptr<PhotoFilters> PhotoFiltersRef;
	}
}