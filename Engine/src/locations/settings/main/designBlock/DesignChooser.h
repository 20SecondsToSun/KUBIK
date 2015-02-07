#pragma once
#include "gui/CompositeDispatcher.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "ImageQuadroButton.h"
#include "SettingsFactory.h"

namespace kubik
{
	namespace config
	{
		class DesignChooser: public CompositeDispatcher
		{
		public:	
			DesignChooser(ConfigSettingsRef configSettings, Vec2i position)
				:CompositeDispatcher(), configSettings(configSettings)					 
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
				loadButton = settingsFactory().createLoadButton(btns[userDesignID]->getLocalPosition() + Vec2f(0, btns[userDesignID]->getHeight()));
			
				//selectActiveDesign(id);		
			}	
			
			void selectActiveDesign(int id)
			{	
				if(activeBtn)
				{
					if(activeBtn->getItem().getID() == userDesignID)
					{
						removeChild(loadButton);
						loadButton->removeMouseUpListener();
					}

					activeBtn->setSelection(false);
				}

				btns[id]->setSelection(true);
				activeBtn = btns[id];

				if(id == userDesignID)
				{
					addChild(loadButton);
					loadButton->addMouseUpListener(&DesignChooser::openSystemDirectory, this);
				}
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->addMouseUpListener(&DesignChooser::mouseUpFunction, this);

				if(activeBtn->getItem().getID() == userDesignID)
					loadButton->addMouseUpListener(&DesignChooser::openSystemDirectory, this);
			}

			virtual void mouseUpFunction(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
				if(typeid(*ev) == typeid(ChangeDesignEvent))
				{		
					ChangeDesignEventRef statEvent = static_pointer_cast<ChangeDesignEvent>(event);	
					int id = statEvent->getItem().getID();
					btns[id]->setSelection(true);

					selectActiveDesign(id);					
				}
			}

			void openSystemDirectory(EventGUIRef& event)
			{
				logger().log("open directory");
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->removeMouseUpListener();	

				loadButton->removeMouseUpListener();
			}

		private:			
			ConfigSettingsRef configSettings;
			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;

			std::map<int, ImageQuadroButtonRef> btns;	

			int userDesignID;
		};

		typedef std::shared_ptr<DesignChooser> DesignChooserRef;
	}
}