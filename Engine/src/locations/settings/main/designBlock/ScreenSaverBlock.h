#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"

namespace kubik
{
	namespace config
	{
		class ScreenSaverBlock: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;

			ScreenSaverBlock(ConfigSettingsRef configSettings, Vec2i position)
				:Sprite(), 
				configSettings(configSettings),
				lineColor(ci::Color::hex(0x233442)),
				lineWidth(2),				
				lineLength(707.0f)
			{
				setPosition(position);	

				loadButton = settingsFactory().createLoadButton(Vec2f(505, 66));
				addChild(loadButton);

				checker = settingsFactory().createScreenSaverChecker(Vec2f(0, 60));
				addChild(checker);
			}

			bool getScreenSaverValue()
			{
				return checker->getValue();
			}

			void activateListeners()
			{
				checker->connectEventHandler(&ScreenSaverBlock::checkerClicked, this);
				loadButton->connectEventHandler(&ScreenSaverBlock::openDirectory, this);
			}

			void unActivateListeners()
			{
				checker->disconnectEventHandler();
				loadButton->disconnectEventHandler();
			}

			void checkerClicked(EventGUIRef event)
			{
				checker->swapActive();	
				
				if(eventHandlerDic[SCREEN_SAVER_STATE])
					eventHandlerDic[SCREEN_SAVER_STATE]();	
			}	

			void openDirectory(EventGUIRef event)
			{
				if(eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER])
					eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER]();	
			}
			
			virtual void drawLayout()
			{
				drawDecorationLine();
				//gl::drawSolidRect(Rectf(0.0f, 0.0f, 707.0f, 200.0f));

				gl::pushMatrices();
				gl::translate(0, 200);	
				drawDecorationLine();
				gl::popMatrices();

				textTools().textFieldDraw(
					configSettings->getTextItem(ConfigTextID::LOADMAIN),				
					ci::Vec2f(171.0f, 57.0f));

				textTools().textFieldDraw(
					configSettings->getTextItem(ConfigTextID::LOADSUB),
					ci::Vec2f(171.0f, 105.0f));			

				gl::color(ci::Color::white());
			}

			void drawDecorationLine()
			{								
				gl::lineWidth(lineWidth);
				gl::color(lineColor);
				gl::drawLine(Vec2f::zero(), Vec2f(lineLength, 0.0f));
				gl::color(Color::white());
			}

			void setAlpha(float alpha)
			{
				//textcolor = Utils::colorAlpha(textcolor, alpha);	
			}

		private:
			ConfigSettingsRef configSettings;
			float lineWidth;
			float lineLength;
			ci::Color lineColor;
			LoadButtonRef loadButton;

			ScreenSaverCheckerRef checker;
		};

		typedef std::shared_ptr<ScreenSaverBlock> ScreenSaverBlockRef;
	}
}