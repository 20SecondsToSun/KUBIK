#pragma once
#include "IDispatcher.h"
#include "IDrawable.h"

namespace kubik
{
	namespace config
	{
		class MenuBlock:public IDrawable, public IDispatcher
		{
		public:	
			MenuBlock():titleText("Меню"), 
						subTitleText("Настройте внешний вид"),					
						titleColor(Color::hex(0xffffff)),
						subTitleColor(Color::hex(0x939eb0))
			{

			}

			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);
				//	gl::drawSolidRect(Rectf(0, 0, 405, 175));
					gl::draw(icon, Vec2f(10, 48));
					textTools().textFieldDraw(titleText, &titleFont, titleColor, Vec2f(82, 40));
					textTools().textFieldDraw(subTitleText, &subTitleFont, subTitleColor, Vec2f(89, 100));
				gl::popMatrices();
			}			

			void setIcon(Texture tex)
			{
				icon = tex;
			}

			void setTitleFont(Font font)
			{
				this->titleFont = font;
			}

			void setSubTitleFont(Font font)
			{
				this->subTitleFont = font;
			}

			void createBtn()
			{
				showPathBtn = ButtonRef(new Button(Rectf(position.x, position.y, position.x + 405, position.y + 175)));	
				connect_once(showPathBtn->mouseUpSignal, bind(&MenuBlock::showPath, this, std::placeholders::_1));
				displayList.push_back(showPathBtn);
			}

			void showPath(IButton& button)
			{			
				console()<<"showPath---menu"<<endl;
			}				

		private:
			Texture icon;
			string titleText, subTitleText;
			Color titleColor, subTitleColor;
			Font titleFont, subTitleFont;
			ButtonRef showPathBtn;
		};

		typedef std::shared_ptr<MenuBlock> MenuBlockRef;
	}
}