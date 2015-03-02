#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "ImageQuadroButton.h"
#include "SettingsFactory.h"

namespace kubik
{
	namespace config
	{
		class DesignChooser: public Sprite
		{
		public:	
			static const int CHANGED_DESIGN = 0;
			static const int OPEN_USER_DESIGN_FOLDER = 1;

			DesignChooser(ConfigSettingsRef configSettings, Vec2i position)
				:Sprite(), configSettings(configSettings)					 
			{
				setPosition(position);	

				DesignData designdata = configSettings->getDesignData();				
				
				Vec2f pos = Vec2f::zero();
				int index = 0;
				float shiftX = 53, shiftY = 130;

				for (auto it : designdata)
				{				 
					Texture icon = configSettings->getTexture(it.getName());
					it.setIcon(icon);
					it.setFont(configSettings->getFonts());

					pos.x = (icon.getWidth() + shiftX) * (index % 3);
					pos.y = (icon.getWidth() + shiftY) * (index / 3);

					ImageQuadroButtonRef imageQuadroButton 
						= settingsFactory().createChangeDesignButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	

					index++;					
				}

				int id = configSettings->getActiveDesignID();
				userDesignID = configSettings->getUserDesignID();
				loadButton = settingsFactory().createDecorLoadButton("  ",btns[userDesignID]->getLocalPosition() + Vec2f(0, btns[userDesignID]->getHeight()));
			
				selectActiveDesign(id);		
			}	
			
			void selectActiveDesign(int id)
			{
				if(activeID == id) return;
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
					//loadButton->connectEventHandler(&DesignChooser::openSystemDirectory, this);
				}
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&DesignChooser::buttonClicked, this);

				if(activeBtn->getItem().getID() == userDesignID)
					loadButton->connectEventHandler(&DesignChooser::openSystemDirectory, this);
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();	

				loadButton->disconnectEventHandler();
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
				if(typeid(*ev) == typeid(ChangeDesignEvent))
				{		
					ChangeDesignEventRef statEvent = static_pointer_cast<ChangeDesignEvent>(event);	
					int id = statEvent->getItem().getID();
					btns[id]->setSelection(true);					

					if(activeID != id && id == userDesignID)
						loadButton->connectEventHandler(&DesignChooser::openSystemDirectory, this);

					selectActiveDesign(id);

					callback(CHANGED_DESIGN);
				}
			}

			int getDesignID() const
			{
				return activeID;
			}

			void openSystemDirectory(EventGUIRef& event)
			{			
				callback(OPEN_USER_DESIGN_FOLDER);
			}		

		private:			
			ConfigSettingsRef configSettings;
			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;

			std::map<int, ImageQuadroButtonRef> btns;	

			int userDesignID;
			int activeID;
		};

		typedef std::shared_ptr<DesignChooser> DesignChooserRef;
	}
}