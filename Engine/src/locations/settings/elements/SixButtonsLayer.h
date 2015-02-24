#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "InstakubSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class SixButtonsLayer> SixButtonsLayerRef;

		class SixButtonsLayer: public Sprite
		{
		public:
			SixButtonsLayer(InstakubSettingsRef settings)
			{
				using namespace ci;

				DesignData designdata = settings->getPhotoCardStyles();				
				int activeID = settings->getActivePhotoCardStyleDesignID();
				userDesignID = settings->getUserPhotoCardStyleDesignID();

				Vec2f pos = Vec2f::zero();
				int i = 0;
				float shiftX = 53, shiftY = 130;
				float startX = 106, startY= 354;

				for (auto it : designdata)
				{				 
					gl::Texture icon = settings->getTexture(it.getName());
					it.setIcon(icon);
					it.setFont(settings->getFonts());

					pos.x = startX + (icon.getWidth() + shiftX) * (i % 3);
					pos.y = startY + (icon.getWidth() + shiftY) * (i / 3);

					ImageQuadroButtonRef imageQuadroButton = settingsFactory().createCardStyleButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	
					i++;					
				}			

				loadButton = settingsFactory().createDecorLoadButton(settings->getUserPhotoCardStylePath(), btns[userDesignID]->getLocalPosition() + Vec2f(0, btns[userDesignID]->getHeight()));			
				selectActiveDesign(activeID);	
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
					loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);
				}
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

			void openSystemDirectory(EventGUIRef& event)
			{
				
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&SixButtonsLayer::buttonClicked, this);

				if(activeBtn->getItem().getID() == userDesignID)
					loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();

				loadButton->disconnectEventHandler();
			}

		private:
			std::map<int, ImageQuadroButtonRef> btns;	
			int userDesignID, activeID;			
			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;
		};
	}
}