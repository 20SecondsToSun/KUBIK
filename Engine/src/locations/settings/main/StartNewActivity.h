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
				 font(configSettings->getFont("helveticaLight24")),
				 text("Завершить и начать новое"),
				 textcolor(Color::hex(0x00f067))				 
			{
				setPosition(position);
				//startNewBtn = SimpleButtonRef(new SimpleButton(Rectf(0, 0, 350.0f, 37.0f)));	
				//addChild(startNewBtn);
			}
			
			virtual void drawLayout()
			{
				gl::color(textcolor);
				gl::draw(icon);					
				textTools().textFieldDraw(text, &font, textcolor, Vec2f(42, -5));			
			}

			void setAlpha(float  alpha)
			{
				textcolor = Utils::colorAlpha(textcolor, alpha);	
			}

			void setIcon(Texture tex)
			{
				icon = tex;
			}

			void setText(string text)
			{
				this->text = text;
			}

			void tryToStartNewActivity(IButton& button)
			{
				console()<<"start new activity"<<endl;
				tryToStartNewActivitySignal(button);
			}			

			ButtonSignal startNewActivitySignal, cancelNewActivityTrySignal, tryToStartNewActivitySignal;

		private:			
			Texture icon;
			string text;
			ColorA textcolor;
			Font font;
			//SimpleButtonRef startNewBtn;			
		};

		typedef std::shared_ptr<StartNewActivity> StartNewActivityRef;
	}
}