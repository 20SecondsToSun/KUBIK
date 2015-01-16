#pragma once

#include "IDrawable.h"
#include "IDispatcher.h"

namespace kubik
{
	namespace config
	{
		class StartNewActivity: public IDispatcher
		{
		public:	
			StartNewActivity():text("Завершить и начать новое"),
						textcolor(Color::hex(0x00f067))
			{

			}
			
			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);
					//gl::drawSolidRect(Rectf(0, 0, 350, 37));
					gl::draw(icon);
					gl::color(textcolor);
					textTools().textFieldDraw(text, &font, textcolor, Vec2f(42, -5));
				gl::popMatrices();						
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

			void createBtn()
			{
				startNewBtn = ButtonRef(new Button(Rectf(position.x, position.y, position.x + 350, position.y + 37)));	
				//connect_once(startNewBtn->mouseUpSignal, bind(&StartNewActivity::tryToStartNewActivity, this, std::placeholders::_1));
				displayList.push_back(startNewBtn);
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
			Color textcolor;
			Font font;
			ButtonRef startNewBtn;			
		};

		typedef std::shared_ptr<StartNewActivity> StartNewActivityRef;
	}
}