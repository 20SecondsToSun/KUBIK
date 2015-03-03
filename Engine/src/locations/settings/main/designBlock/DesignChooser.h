#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "ImageQuadroButton.h"
#include "SettingsFactory.h"
#include "SixButtonsLayer.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class SixButtonsLayer<ChangeDesignEvent>> SixButtonsLayerDesignRef;
		typedef std::shared_ptr<class DesignChooser> DesignChooserRef;

		class DesignChooser: public Sprite
		{
		public:	
			static const int CHANGED_DESIGN = 0;
			static const int OPEN_USER_DESIGN_FOLDER = 1;

			DesignChooser(ConfigSettingsRef configSettings, const ci::Vec2i& position)
				:Sprite(), configSettings(configSettings)					 
			{
				setPosition(position);

				DesignData designdata = configSettings->getDesignData();	
				int activeID		  = configSettings->getActiveDesignID();
				int userDesignID	  = configSettings->getUserDesignID();
				std::string syspath	  = configSettings->getUserDesignPath();
	
				sixBtnLayer = SixButtonsLayerDesignRef(new SixButtonsLayer<ChangeDesignEvent>(designdata, activeID, userDesignID, syspath, 0.0f, 0.0f));
				addChild(sixBtnLayer);				
			}

			void activateListeners()
			{
				sixBtnLayer->connectEventHandler(&DesignChooser::buttonClicked, this);
				Sprite::activateListeners();
			}

			void unActivateListeners()
			{
				sixBtnLayer->disconnectEventHandler();
				Sprite::unActivateListeners();
			}		

			void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
				if(ev)
				{
					console()<<"buttonClicked design changed"<<endl;
					callback(CHANGED_DESIGN);
					//mouseUpSignal(event);
				}
			}

			private:			
				ConfigSettingsRef configSettings;
				SixButtonsLayerDesignRef sixBtnLayer;
		};		
	}
}