#pragma once
#include "IDispatcher.h"

namespace kubik
{
	namespace config
	{
		class ScreenSaverBlock: public IDispatcher
		{
		public:	
			ScreenSaverBlock(ConfigSettingsRef configSettings, Vec2i position)
						:titleText("Заставка"), 
						subTitleText("Выберите видео-файл"),					
						titleColor(Color::hex(0xffffff)),
						subTitleColor(Color::hex(0x939eb0)),
						titleFont(configSettings->getFont("introLight44")),
						subTitleFont(configSettings->getFont("helveticaLight22")),
						icon(configSettings->getTexture("ssIcon")),
						iconColor(Color::white())
			{
				setPosition(position);
				showPathBtn = ButtonRef(new Button(Rectf(position.x, position.y, position.x + 405, position.y + 175)));	
				displayList.push_back(showPathBtn);
			}

			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);	
					gl::color(iconColor);
					gl::draw(icon, Vec2f(10, 48));
					textTools().textFieldDraw(titleText, &titleFont, titleColor, Vec2f(82, 40));
					textTools().textFieldDraw(subTitleText, &subTitleFont, subTitleColor, Vec2f(89, 100));
				gl::popMatrices();
			}

			void setAlpha(float  alpha)
			{
				titleColor = ColorA(titleColor.r, titleColor.g, titleColor.b, alpha);	
				subTitleColor = ColorA(subTitleColor.r, subTitleColor.g, subTitleColor.b, alpha);	
				iconColor = ColorA(iconColor.r, iconColor.g, iconColor.b, alpha);	
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

			void showPath(IButton& button)
			{			
				console()<<"showPath---screensaver"<<endl;
			}

		private:			
			Texture icon;
			string titleText, subTitleText;
			ColorA titleColor, subTitleColor, iconColor;
			Font titleFont, subTitleFont;

			ButtonRef showPathBtn;
		};

		typedef std::shared_ptr<ScreenSaverBlock> ScreenSaverBlockRef;
	}
}