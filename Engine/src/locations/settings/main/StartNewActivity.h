#pragma once

#include "IDispatcher.h"

namespace kubik
{
	namespace config
	{
		class StartNewActivity: public IDispatcher
		{
		public:	
			StartNewActivity(ConfigSettingsRef configSettings, Vec2i position)
				:icon(configSettings->getTexture("iconStartNew")),
				 font(configSettings->getFont("helveticaLight24")),
				 text("Завершить и начать новое"),
				 textcolor(Color::hex(0x00f067))
			{
				setPosition(position);
				startNewBtn = ButtonRef(new Button(Rectf(position, position + Vec2f(350.0f, 37.0f))));	
				displayList.push_back(startNewBtn);
			}
			
			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);
					gl::color(textcolor);
					gl::draw(icon);					
					textTools().textFieldDraw(text, &font, textcolor, Vec2f(42, -5));
				gl::popMatrices();						
			}

			void setAlpha(float  alpha)
			{
				textcolor = ColorA(textcolor.r, textcolor.g, textcolor.b, alpha);
			}

			void setIcon(Texture tex)
			{
				icon = tex;
			}

			void setFont(Font font)
			{
				this->font = font;
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
			ButtonRef startNewBtn;			
		};

		typedef std::shared_ptr<StartNewActivity> StartNewActivityRef;
	}
}