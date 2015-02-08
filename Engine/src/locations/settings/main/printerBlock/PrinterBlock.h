#pragma once
#include "gui/Sprite.h"
#include "gui/ImageButtonSprite.h"
#include "PrinterControls.h"

namespace kubik
{
	namespace config
	{
		class PrinterBlock: public Sprite
		{
		public:	
			static const int OPEN_EVENT = 0;
			static const int HIDE_EVENT = 1;
			static const int STAT_RESET_EVENT = 2;
			static const int OPENED = 3;
			static const int HIDED = 4;

			PrinterBlock(ConfigSettingsRef configSettings, Vec2i position)
				:Sprite(),
				hintText(configSettings->getTextItem(ConfigTextID::PHOTO_LEFT)),
				changeBtnText(configSettings->getTextItem(ConfigTextID::CATRINGE_CHANGED)),
				iconColor(Color::hex(0xffffff)),
				numsColor(Color::hex(0xffffff)),
				bckgrndColor(Color::hex(0x1a1827)),
				barColor1(Color::hex(0x233442)),
				barColor2(Color::hex(0x00f067)),
				maxBarWidth(312.0f),
				numsFont(configSettings->getFont("introBold72")),
				icon(configSettings->getTexture("catridgeIcon")),
				maxPhotosToPrint(configSettings->getData().maxPhotosToPrint)						
			{				
				setPosition(position);

				Texture img = textTools().getTextField(changeBtnText);
				openBtn = ImageButtonSpriteRef(new ImageButtonSprite(img, Vec2f(670, 62.5)));
				addChild(openBtn);

				controls = PrinterControlsRef(new PrinterControls(configSettings, Vec2f(0, 170)));
				addChild(controls);	
			}

			virtual void activateListeners()
			{
				openBtn->connectEventHandler(&PrinterBlock::openButtonHandler, this);
			}

			void openButtonHandler(EventGUIRef& event)
			{
				if(eventHandlerDic[OPEN_EVENT])
				{
					openBtn->disconnectEventHandler();
					eventHandlerDic[OPEN_EVENT]();				
				}
			}

			virtual void unActivateListeners()
			{
				openBtn->disconnectEventHandler();
			}

			virtual void drawLayout()
			{				
				gl::color(bckgrndColor);
				gl::drawSolidRect(Rectf(0.0f, 0.0f, 835.0f, 170.0f));
				gl::color(Color::white());
				textTools().textFieldDraw(to_string(currentPhotosPrinted), &numsFont, numsColor, Vec2f(88, 26));
				textTools().textFieldDraw(hintText, Vec2f(240, 42.5));
			
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

				openBtn->draw();
				controls->draw();
			}

			void openControls(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + Vec2f(0, -400), time, eFunc)
					     .updateFn(bind( &PrinterBlock::posAnimationUpdate, this))
						 .finishFn(bind( &PrinterBlock::openControlsAnimationFinish, this));
			}

			void closeControls(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + Vec2f(0, 400), time, eFunc)
					     .updateFn(bind( &PrinterBlock::posAnimationUpdate, this))
						 .finishFn(bind( &PrinterBlock::hideControlsAnimationFinish, this));
			}			

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

			void hideControlsAnimationFinish()
			{				
				controls->unActivateListeners();
				eventHandlerDic[HIDED]();
			}

			void openControlsAnimationFinish()
			{	
				if(eventHandlerDic[OPENED])
				{
					eventHandlerDic[OPENED]();
					controls->activateListeners();
					controls->connectEventHandler(&PrinterBlock::eventListener, this);
				}
			}
			virtual void eventListener(EventGUIRef event)
			{
				EventGUI *ev = event.get();		
				if(!ev) return;	

				if(typeid(*ev) == typeid(PrinterControlsHideEvent))
				{
					if(eventHandlerDic[HIDE_EVENT])					
						eventHandlerDic[HIDE_EVENT]();					
				}
				else if(typeid(*ev) == typeid(PrinterStatResetEvent))
				{
					if(eventHandlerDic[STAT_RESET_EVENT])					
						eventHandlerDic[STAT_RESET_EVENT]();					
				}

			}

			void setAlpha(float  alpha)
			{			
				bckgrndColor = Utils::colorAlpha(bckgrndColor, alpha);
				numsColor = Utils::colorAlpha(numsColor, alpha);
				hintText.setColor(Utils::colorAlpha(hintText.getColor(), alpha));
				barColor1 = Utils::colorAlpha(barColor1, alpha);
				barColor2 = Utils::colorAlpha(barColor2, alpha);
				iconColor = Utils::colorAlpha(iconColor, alpha);
				Sprite::setAlpha(alpha);
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
		
		private:
			Vec2i position;
			Texture icon;
			TextItem hintText, changeBtnText;
			ColorA numsColor, bckgrndColor, barColor2, barColor1, iconColor;
			Font numsFont;
			int maxPhotosToPrint;
			int currentPhotosPrinted;
			float maxBarWidth;
			int curBarWidth;
		
			Rectf btnRectf;
			ImageButtonSpriteRef openBtn;
			Anim<Vec2f> animatePosition;
			PrinterControlsRef controls;				
		};

		typedef std::shared_ptr<PrinterBlock> PrinterBlockRef;
	}
}