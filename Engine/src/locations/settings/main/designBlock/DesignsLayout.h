#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "main/DesignBlock/ReturnButton.h"
#include "main/DesignBlock/ScreenSaverBlock.h"
#include "ConfigSettings.h"
#include "DesignChooser.h"

namespace kubik
{
	namespace config
	{
		class DesignsLayout: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;
			static const int CHANGED_DESIGN = 2;
			static const int OPEN_USER_DESIGN_FOLDER = 3;	
			static const int HIDE = 4;	

			DesignsLayout(ConfigSettingsRef configSettings, Vec2i position)
				:Sprite(), configSettings(configSettings)					 
			{
				setPosition(position);	

				returnButton = ReturnButtonRef(new ReturnButton(configSettings, ci::Vec2f(0, 690 + 201)));	
				addChild(returnButton);	

				screenSaver = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, ci::Vec2f(0, 690)));	
				addChild(screenSaver);	

				designChooser = DesignChooserRef(new DesignChooser(configSettings, ci::Vec2f(0, 2)));	
				addChild(designChooser);
			}	
			
			virtual void activateListeners()
			{
				designChooser->activateListeners();
				screenSaver->activateListeners();

				designChooser->connectEventHandler(&DesignsLayout::designChanged,		 this, DesignChooser::CHANGED_DESIGN);
				designChooser->connectEventHandler(&DesignsLayout::openUserDesignFolder, this, DesignChooser::OPEN_USER_DESIGN_FOLDER);
				screenSaver->connectEventHandler(&DesignsLayout::checkerChanged,		 this, ScreenSaverBlock::SCREEN_SAVER_STATE);
				screenSaver->connectEventHandler(&DesignsLayout::openScreenSaverFolder,	 this, ScreenSaverBlock::SCREEN_SAVER_OPEN_FOLDER);
				returnButton->connectEventHandler(&DesignsLayout::returnHandler, this);			
			}

			virtual void unActivateListeners()
			{
				designChooser->unActivateListeners();
				designChooser->disconnectEventHandler(DesignChooser::CHANGED_DESIGN);
				designChooser->disconnectEventHandler(DesignChooser::OPEN_USER_DESIGN_FOLDER);

				screenSaver->unActivateListeners();
				screenSaver->disconnectEventHandler(ScreenSaverBlock::SCREEN_SAVER_STATE);
				screenSaver->disconnectEventHandler(ScreenSaverBlock::SCREEN_SAVER_OPEN_FOLDER);

				returnButton->disconnectEventHandler();	
			}

			void checkerChanged()
			{
				if(eventHandlerDic[SCREEN_SAVER_STATE])
					eventHandlerDic[SCREEN_SAVER_STATE]();	
			}

			void openScreenSaverFolder()
			{
				if(eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER])
					eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER]();	
			}

			void returnHandler(EventGUIRef& event)
			{
				if(eventHandlerDic[HIDE])
					eventHandlerDic[HIDE]();	
			}			

			void designChanged()
			{
				if(eventHandlerDic[CHANGED_DESIGN])
					eventHandlerDic[CHANGED_DESIGN]();	
			}

			void openUserDesignFolder()
			{
				if(eventHandlerDic[OPEN_USER_DESIGN_FOLDER])
					eventHandlerDic[OPEN_USER_DESIGN_FOLDER]();	
			}

			bool getScreenSaverValue()
			{
				return screenSaver->getScreenSaverValue();
			}

			int getDesignID()
			{
				return designChooser->getDesignID();
			}

		private:			
			ConfigSettingsRef	configSettings;
			ScreenSaverBlockRef screenSaver;
			ReturnButtonRef		returnButton;
			DesignChooserRef	designChooser;
		};

		typedef std::shared_ptr<DesignsLayout> DesignsLayoutRef;
	}
}