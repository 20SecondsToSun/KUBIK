#pragma once
#include "gui/Dispatcher.h"
#include "TextTools.h"

namespace kubik
{
	namespace config
	{
		class StartNewActivity: public SimpleButton
		{
		public:	
			StartNewActivity(ConfigSettingsRef configSettings, Vec2i position)
				:SimpleButton(Rectf(0, 0, 350.0f, 37.0f)),
				icon(configSettings->getTexture("iconStartNew")),				
				textItem(configSettings->getTextItem(ConfigTextID::FINANDSTART)),
				color(configSettings->getTextItem(ConfigTextID::FINANDSTART).getColor())
			{
				setPosition(position);
			}
			
			virtual void drawLayout()
			{
				gl::color(color);
				gl::draw(icon);					
				textTools().textFieldDraw(textItem, Vec2f(42, -5));			
			}

			void setAlpha(float  alpha)
			{
				color = Utils::colorAlpha(color, alpha);	
			}

			void tryToStartNewActivity(IButton& button)
			{
				console()<<"start new activity"<<endl;
				tryToStartNewActivitySignal(button);
			}			

			ButtonSignal tryToStartNewActivitySignal;

		private:			
			ci::gl::Texture icon;
			TextItem textItem;
			ci::ColorA color;
		};

		typedef std::shared_ptr<StartNewActivity> StartNewActivityRef;
	}
}