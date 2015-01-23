#pragma once
#include "gui/Dispatcher.h"
#include "gui/ImageButton.h"

namespace kubik
{
	namespace config
	{
		class PrinterBlock: public CompositeDispatcher
		{
		public:	
			PrinterBlock(ConfigSettingsRef configSettings, Vec2i position)
				:CompositeDispatcher(),
				hintText("ÙÓÚÓ ‰Ó Á‡ÏÂÌ˚ Í‡ÚË‰Ê‡"), 
						changeBtnText("«‡ÏÂÌÂÌ"),						
						iconColor(Color::hex(0xffffff)),
						hintColor(Color::hex(0xffffff)),
						numsColor(Color::hex(0xffffff)),
						changeBtnColor(Color::hex(0x00f067)),
						bckgrndColor(Color::hex(0x1a1827)),
						barColor1(Color::hex(0x233442)),
						barColor2(Color::hex(0x00f067)),
						maxBarWidth(312.0f),
						numsFont(configSettings->getFont("introBold72")),
						hintFont(configSettings->getFont("helveticaLight24")),
						changeBtnFont(configSettings->getFont("helveticaLight24")),
						icon(configSettings->getTexture("catridgeIcon")),
						maxPhotosToPrint(configSettings->getData().maxPhotosToPrint)						
			{				
				//btnRectf = Rectf(Vec2f(620, 66) + position, position + Vec2f(620 + 155, 66 + 35));
				//resetBtn = ButtonRef(new Button(btnRectf));					
				setPosition(position);

				Texture img = textTools().getTextField(changeBtnText, &changeBtnFont, changeBtnColor);
				changeBtn = ImageButtonRef(new ImageButton(img, Vec2f(670, 62.5)));
				addChild(changeBtn);
			}

			virtual void drawLayout()
			{				
				gl::color(bckgrndColor);
				gl::drawSolidRect(Rectf(0.0f, 0.0f, 835.0f, 170.0f));
				gl::color(Color::white());
				textTools().textFieldDraw(to_string(currentPhotosPrinted), &numsFont, numsColor, Vec2f(88, 26));
				textTools().textFieldDraw(hintText, &hintFont, hintColor, Vec2f(240, 42.5));
			
				gl::color(iconColor);
				gl::draw(icon, Vec2f(620, 66));
				gl::color(barColor1);
				gl::drawSolidRoundedRect(Rectf(245, 90, 245 + maxBarWidth, 103), 8, 200);
				gl::color(barColor2);
				gl::drawSolidRoundedRect(Rectf(245, 90, 245 + curBarWidth, 103), 8, 200);
				gl::color(Color::white());		
			}

			virtual void draw()
			{				
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
					drawLayout();
				gl::popMatrices();
				CompositeDispatcher::draw();
			}

			void setAlpha(float  alpha)
			{			
				bckgrndColor = Utils::colorAlpha(bckgrndColor, alpha);
				numsColor = Utils::colorAlpha(numsColor, alpha);
				hintColor = Utils::colorAlpha(hintColor, alpha);
				changeBtnColor = Utils::colorAlpha(changeBtnColor, alpha);
				barColor1 = Utils::colorAlpha(barColor1, alpha);
				barColor2 = Utils::colorAlpha(barColor2, alpha);
				iconColor = Utils::colorAlpha(iconColor, alpha);
				CompositeDispatcher::setAlpha(alpha);
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
		
			Rectf btnRectf;
			ImageButtonRef changeBtn;
		};

		typedef std::shared_ptr<PrinterBlock> PrinterBlockRef;
	}
}