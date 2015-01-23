#pragma once
#include "gui/Dispatcher.h"

namespace kubik
{
	namespace config
	{
		class MenuBlock:public Dispatcher
		{
		public:	
			MenuBlock(ConfigSettingsRef configSettings, Vec2i position)
						:titleText("Меню"), 
						subTitleText("Настройте внешний вид"),					
						titleColor(Color::hex(0xffffff)),
						subTitleColor(Color::hex(0x939eb0)),
						titleFont(configSettings->getFont("introLight44")),
						subTitleFont(configSettings->getFont("helveticaLight22")),
						icon(configSettings->getTexture("menuIcon")),
						iconColor(Color::white())
			{
				//showPathBtn = ButtonRef(new Button(Rectf(position, position + Vec2f(405, 175))));	
				setPosition(position);
			}

			virtual void drawLayout()
			{
				gl::color(iconColor);
				gl::draw(icon, Vec2f(10, 48));
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
				console()<<"showPath---menu"<<endl;
			}				

		private:
			Texture icon;
			string titleText, subTitleText;
			ColorA titleColor, subTitleColor, iconColor;
			Font titleFont, subTitleFont;
			ButtonRef showPathBtn;
		};

		typedef std::shared_ptr<MenuBlock> MenuBlockRef;
	}
}