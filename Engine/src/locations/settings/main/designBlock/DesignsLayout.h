#pragma once
#include "gui/CompositeDispatcher.h"
#include "TextTools.h"
#include "main/DesignBlock/ReturnButton.h"
#include "main/DesignBlock/ScreenSaverBlock.h"
#include "ConfigSettings.h"
#include "DesignChooser.h"

namespace kubik
{
	namespace config
	{
		class DesignsLayout: public CompositeDispatcher
		{
		public:	
			DesignsLayout(ConfigSettingsRef configSettings, Vec2i position)
				:CompositeDispatcher(), configSettings(configSettings)					 
			{
				setPosition(position);	

				returnButton = ReturnButtonRef(new ReturnButton(configSettings, ci::Vec2f(0, 201)));	
				//addChild(returnButton);	

				screenSaver = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, ci::Vec2f(0, 2)));	
				screenSaver->addMouseUpListener(&DesignsLayout::mouseUpFunction, this);							
				//addChild(screenSaver);	

				designChooser = DesignChooserRef(new DesignChooser(configSettings, ci::Vec2f(0, 2)));	
				designChooser->addMouseUpListener(&DesignsLayout::mouseUpFunction, this);							
				addChild(designChooser);
			}	
			
			virtual void activateListeners()
			{
				designChooser->activateListeners();
				screenSaver->activateListeners();				
				returnButton->addMouseUpListener(&DesignsLayout::mouseUpFunction, this);			
			}

			virtual void unActivateListeners()
			{
				designChooser->unActivateListeners();
				screenSaver->unActivateListeners();
				returnButton->removeMouseUpListener();
			}

		private:			
			ConfigSettingsRef configSettings;
			ScreenSaverBlockRef screenSaver;
			ReturnButtonRef returnButton;
			DesignChooserRef designChooser;
		};

		typedef std::shared_ptr<DesignsLayout> DesignsLayoutRef;
	}
}