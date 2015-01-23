#pragma once
#include "gui/Dispatcher.h"
#include "TextTools.h"

namespace kubik
{
	namespace config
	{
		class ScreenSaverBlock: public Dispatcher
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
				//showPathBtn = ButtonRef(new Button(Rectf(position.x, position.y, position.x + 405, position.y + 175)));					
			}

			virtual void drawLayout()
			{
				gl::color(iconColor);
				gl::draw(icon, Vec2f(10.0f, 48.0f));
				textTools().textFieldDraw(titleText, &titleFont, titleColor, Vec2f(82, 40));
				textTools().textFieldDraw(subTitleText, &subTitleFont, subTitleColor, Vec2f(89, 100));			
			}

			void setAlpha(float  alpha)
			{
				titleColor = Utils::colorAlpha(titleColor, alpha);	
				subTitleColor = Utils::colorAlpha(subTitleColor, alpha);	
				iconColor = Utils::colorAlpha(iconColor, alpha);	
			}

			void setIcon(Texture tex)
			{
				icon = tex;
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