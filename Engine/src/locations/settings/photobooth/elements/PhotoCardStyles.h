#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoCardStyles: public IPhotoboothItem
		{
		public:	
			PhotoCardStyles(PhotoboothSettingsRef phbSettings, ci::Color color, int index)
				:IPhotoboothItem(phbSettings, PhtTextID::CARD_STYLE, color, index)
			{
				DesignData designdata = settings->getPhotoOverDesignData();				

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

					ImageQuadroButtonRef imageQuadroButton = settingsFactory().createCardStyleButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	
					i++;					
				}

				int id = settings->getActivePhotoCardStyleDesignID();
				userDesignID = settings->getUserPhotoCardStyleDesignID();
				loadButton = settingsFactory().createDecorLoadButton(btns[userDesignID]->getLocalPosition() + Vec2f(0, btns[userDesignID]->getHeight()));			
				selectActiveDesign(id);	
			}	

			void selectActiveDesign(int id)
			{
				activeID = id;

				if(activeBtn)
				{
					if(activeBtn->getItem().getID() == userDesignID)
					{
						removeChild(loadButton);
						loadButton->disconnectEventHandler();
					}

					activeBtn->setSelection(false);
				}

				btns[id]->setSelection(true);
				activeBtn = btns[id];

				if(id == userDesignID)
				{
					addChild(loadButton);
					loadButton->connectEventHandler(&PhotoCardStyles::openSystemDirectory, this);
				}
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&PhotoCardStyles::buttonClicked, this);

				if(activeBtn->getItem().getID() == userDesignID)
					loadButton->connectEventHandler(&PhotoCardStyles::openSystemDirectory, this);

				IPhotoboothItem::activateListeners();
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();	

				loadButton->disconnectEventHandler();
				IPhotoboothItem::unActivateListeners();
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if(typeid(*ev) == typeid(ChangePhotoCardStyleDesignEvent))
				{		
					ChangePhotoCardStyleDesignEventRef statEvent = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(event);	
					int id = statEvent->getItem().getID();
					btns[id]->setSelection(true);
					selectActiveDesign(id);	
				}
			}

			int getDesignID()
			{
				return activeID;
			}

			void openSystemDirectory(EventGUIRef& event)
			{
				console()<<"openSystemDirectory "<<endl;
			}

		private:
			std::map<int, ImageQuadroButtonRef> btns;	
			int userDesignID;
			int activeID;

			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;
		};

		typedef std::shared_ptr<PhotoCardStyles> PhotoCardStylesRef;
	}
}