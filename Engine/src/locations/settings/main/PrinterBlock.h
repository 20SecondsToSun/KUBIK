#pragma once
#include "IDispatcher.h"

namespace kubik
{
	namespace config
	{
		class PrinterBlock: public IDispatcher
		{
		public:	
			PrinterBlock(ConfigSettingsRef configSettings, Vec2i position)
				:hintText("ÙÓÚÓ ‰Ó Á‡ÏÂÌ˚ Í‡ÚË‰Ê‡"), 
						changeBtnText("«‡ÏÂÌÂÌ"),						
						iconColor(Color::hex(0xffffff)),
						hintColor(Color::hex(0xffffff)),
						numsColor(Color::hex(0xffffff)),
						changeBtnColor(Color::hex(0x00f067)),
						bckgrndColor(Color::hex(0x1a1827)),
						maxBarWidth(312.0f),
						numsFont(configSettings->getFont("introBold72")),
						hintFont(configSettings->getFont("helveticaLight24")),
						changeBtnFont(configSettings->getFont("helveticaLight24")),
						icon(configSettings->getTexture("catridgeIcon")),
						maxPhotosToPrint(configSettings->getData().maxPhotosToPrint),
						barColor1(Color::hex(0x233442)),
						barColor2(Color::hex(0x00f067))
			{
				btnRectf = Rectf(Vec2f(620, 66) + position, position + Vec2f(620 + 155, 66 + 35));
				resetBtn = ButtonRef(new Button(btnRectf));	
				displayList.push_back(resetBtn);
				setPosition(position);
			}

			virtual void draw()
			{				
				gl::pushMatrices();
					gl::translate(position);
					gl::color(bckgrndColor);
					gl::drawSolidRect(Rectf(0.0f, 0.0f, 835.0f, 170.0f));
					gl::color(Color::white());
					textTools().textFieldDraw(to_string(currentPhotosPrinted), &numsFont, numsColor, Vec2f(88, 26));
					textTools().textFieldDraw(hintText, &hintFont, hintColor, Vec2f(240, 42.5));
					textTools().textFieldDraw(changeBtnText, &changeBtnFont, changeBtnColor, Vec2f(670, 62.5));
					gl::color(iconColor);
					gl::draw(icon, Vec2f(620, 66));
					gl::color(barColor1);
					gl::drawSolidRoundedRect(Rectf(245, 90, 245 + maxBarWidth, 103), 8, 200);
					gl::color(barColor2);
					gl::drawSolidRoundedRect(Rectf(245, 90, 245 + curBarWidth, 103), 8, 200);
				gl::popMatrices();	
			}

			void setAlpha(float  alpha)
			{
				bckgrndColor = ColorA(bckgrndColor.r, bckgrndColor.g, bckgrndColor.b, alpha);
				numsColor = ColorA(numsColor.r,      numsColor.g, numsColor.b, alpha);	
				hintColor = ColorA(hintColor.r,      hintColor.g, hintColor.b, alpha);	
				changeBtnColor = ColorA(changeBtnColor.r, changeBtnColor.g, changeBtnColor.b, alpha);	
				barColor1 = ColorA(barColor1.r, barColor1.g, barColor1.b, alpha);	
				barColor2 = ColorA(barColor2.r, barColor2.g, barColor2.b, alpha);	
				iconColor = ColorA(iconColor.r, iconColor.g, iconColor.b, alpha);	
			}

			void setPosition(Vec2i position)
			{
				this->position = position;
			}			

			void setNumsFont(Font font)
			{
				this->numsFont = font;
			}

			void setHintFont(Font font)
			{
				this->hintFont = font;
			}

			void setChangeBtnFont(Font font)
			{
				this->changeBtnFont = font;
			}

			void setChangeBtnIcon(Texture tex)
			{
				icon = tex;
			}

			void setMaxPhotosToPrint(int value)
			{
				maxPhotosToPrint = value;
			}

			void set—urrentPhotosPrinted(int value)
			{
				currentPhotosPrinted = value;
				curBarWidth = ((float)currentPhotosPrinted / maxPhotosToPrint) * maxBarWidth;
			}

			void showPath(IButton& button)
			{			
				console()<<"resetBtn---resetBtn"<<endl;
			}			

		private:
			Vec2i position;
			Texture icon;
			string hintText, changeBtnText;
			ColorA numsColor, hintColor, changeBtnColor, bckgrndColor, barColor2, barColor1, iconColor;
			Font numsFont, hintFont, changeBtnFont;
			int maxPhotosToPrint;
			int currentPhotosPrinted;

			float maxBarWidth;
			int curBarWidth;

			ButtonRef resetBtn;

			Rectf btnRectf;
		};

		typedef std::shared_ptr<PrinterBlock> PrinterBlockRef;
	}
}