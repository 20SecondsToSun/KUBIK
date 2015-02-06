#pragma once
#include "gui/CompositeDispatcher.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"

namespace kubik
{
	namespace config
	{
		class ScreenSaverBlock: public CompositeDispatcher
		{
		public:	
			ScreenSaverBlock(ConfigSettingsRef configSettings, Vec2i position)
				:CompositeDispatcher(), 
				configSettings(configSettings),
				lineWidth(2),
				lineColor(ci::Color::hex(0x233442)),
				lineLength(707.0f)
			{
				setPosition(position);	

				loadButton = settingsFactory().createLoadButton(Vec2f(505, 66));
				addChild(loadButton);

				checker = settingsFactory().createScreenSaverChecker(Vec2f(0, 60));
				addChild(checker);
			}

			void activateListeners()
			{
				checker->addMouseUpListener(&ScreenSaverBlock::checkerClicked, this);
				loadButton->addMouseUpListener(&ScreenSaverBlock::openDirectory, this);
				console()<<"activate checkers"<<endl;
			}

			void checkerClicked(EventGUIRef event)
			{
				checker->swapActive();								
				//mouseUpSignal(event);
			}

			void unActivateListeners()
			{
				checker->removeMouseUpListener();
				loadButton->removeMouseUpListener();
				console()<<"unactivate checkers"<<endl;
			}

			void openDirectory(EventGUIRef event)
			{
				console()<<"open directory"<<endl;
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
					ci::Vec2f(171.0f, 55.0f));

				textTools().textFieldDraw(
					configSettings->getTextItem(ConfigTextID::LOADSUB),
					ci::Vec2f(171.0f, 110.0f));			

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