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
		template <class Type> class SixButtonsLayer: public Sprite
		{
		public:
			SixButtonsLayer(const DesignData& designdata,
				int activeID, 
				int userDesignID,
				const std::string& path,
				const ci::gl::Texture& over = ci::gl::Texture(), 
				float startX = 106,
				float startY = 354)
				:userDesignID(userDesignID),
				activeID(-1)
			{
				using namespace ci;				

				Vec2f pos = Vec2f::zero();
				int i = 0;
				float shiftX = 53, shiftY = 130;
			
				for (auto it : designdata)
				{					
					pos.x = startX + (it.getIcon().getWidth() + shiftX) * (i % 3);
					pos.y = startY + (it.getIcon().getWidth() + shiftY) * (i / 3);

					ImageQuadroButtonRef imageQuadroButton = std::shared_ptr<SixItemButton<Type>>(new SixItemButton<Type>(it, over, pos));
					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	
					i++;					
				}			

				loadButton = settingsFactory().createDecorLoadButton(path, btns[userDesignID]->getLocalPosition() + Vec2f(0.0f, btns[userDesignID]->getHeight()), over);
				selectActiveDesign(activeID);	
			}
			
			void selectActiveDesign(int id)
			{
				if (activeID == id) return;

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

				if(id == userDesignID)// && !hasChild(loadButton))
				{
					addChild(loadButton);
					//loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);
				}
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if(typeid(*ev) == typeid(Type))
				{		
					//ChangePhotoCardStyleDesignEventRef statEvent = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(event);	
					int id = static_pointer_cast<Type>(event)->getItem().getID();
					btns[id]->setSelection(true);
					
					if(activeID != id && id == userDesignID)
						loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);

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