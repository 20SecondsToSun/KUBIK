#pragma once
#include "IDispatcher.h"
#include "IDrawable.h"

namespace kubik
{
	namespace config
	{
		class PrinterBlock: public IDispatcher
		{
		public:	
			PrinterBlock():hintText("ôîòî äî çàìåíû êàðòðèäæà"), 
						changeBtnText("Çàìåíåí"),					
						hintColor(Color::hex(0xffffff)),
						numsColor(Color::hex(0xffffff)),
						changeBtnColor(Color::hex(0x00f067)),
						bckgrndColor(Color::hex(0x1a1827)),
						maxBarWidth(312.0f)
			{
				
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
					gl::draw(icon, Vec2f(620, 66));
					gl::color(Color::hex(0x233442));
					gl::drawSolidRoundedRect(Rectf(245, 90, 245 + maxBarWidth, 103), 8, 200);
					gl::color(Color::hex(0x00f067));
					gl::drawSolidRoundedRect(Rectf(245, 90, 245 + curBarWidth, 103), 8, 200);
				gl::popMatrices();
				//gl::color(ColorA(1,1,1,0.6));
				//gl::drawSolidRect(btnRectf);
			}

			void setPosition(Vec2i position)
			{
				this->position = position;
			}

			void setChangeBtnIcon(Texture tex)
			{
				icon = tex;
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

			void setMaxPhotosToPrint(int value)
			{
				maxPhotosToPrint = value;
			}

			void setÑurrentPhotosPrinted(int value)
			{
				currentPhotosPrinted = value;
				curBarWidth = ((float)currentPhotosPrinted / maxPhotosToPrint) * maxBarWidth;
			}

			void createBtn()
			{
				btnRectf = Rectf(620 + position.x, 66 + position.y, 620 + position.x + 155, 66 + position.y + 35);

				resetBtn = ButtonRef(new Button(btnRectf));	
				//connect_once(resetBtn->mouseUpSignal, bind(&PrinterBlock::showPath, this, std::placeholders::_1));
				displayList.push_back(resetBtn);
			}

			void showPath(IButton& button)
			{			
				console()<<"resetBtn---resetBtn"<<endl;
			}			

		private:
			Vec2i position;
			Texture icon;
			string hintText, changeBtnText;
			Color numsColor, hintColor, changeBtnColor, bckgrndColor;
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