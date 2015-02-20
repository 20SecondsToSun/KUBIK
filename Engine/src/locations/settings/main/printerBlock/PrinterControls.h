#pragma once
#include "gui/Sprite.h"
#include "ConfigSettings.h"
#include "PrinterControlsHideEvent.h"
#include "PrinterStatResetEvent.h"

namespace kubik
{
	namespace config
	{
		class PrinterControls: public Sprite
		{
		public:	
			PrinterControls(ConfigSettingsRef configSettings, ci::Vec2i position)
				:Sprite(), configSettings(configSettings),
				titleText1(configSettings->getTextItem(ConfigTextID::PRINTER_ASK_TITLE)),
				titleText2(configSettings->getTextItem(ConfigTextID::PRINTER_ASK_SUB_TITLE)),		
				iErrorText(configSettings->getTextItem(ConfigTextID::PRINTER_CLOSE_TITLE)),		
				changedText(configSettings->getTextItem(ConfigTextID::PRINTER_OK_TITLE)),		
				closeIcon(configSettings->getTexture("closePrinterIcon"))	
			{
				setPosition(position);
				okButtonArea = Rectf(632, 230, 632 + 285, 230 + 70);

				Texture tex = textTools().getTextField(changedText);
				okTextPos = Vec2f( okButtonArea.x1 + 0.5 * (okButtonArea.getWidth() - tex.getWidth()),
								   okButtonArea.y1 + 0.5 * (okButtonArea.getHeight() - tex.getHeight()) - 2);
			
				PrinterStatResetEventRef  eventReset = PrinterStatResetEventRef(new PrinterStatResetEvent());
				okBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(okButtonArea, eventReset));				
				okBtn->setAlpha(0.5f);
				addChild(okBtn);

				PrinterControlsHideEventRef eventHide = PrinterControlsHideEventRef(new PrinterControlsHideEvent());
				closeBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(415, 40, Vec2f(160, 248), eventHide));				
				closeBtn->setAlpha(0.5f);
				addChild(closeBtn);	

				closeBtnBig = SimpleSpriteButtonRef(new SimpleSpriteButton(getWindowWidth(), 1920 - 400 - 170, Vec2f(0, -(1920 - 400)), eventHide));				
				closeBtnBig->setAlpha(0.5f);
				addChild(closeBtnBig);	
			}			

			virtual void activateListeners()
			{
				okBtn->connectEventHandler(&PrinterControls::eventListener, this);
				closeBtnBig->connectEventHandler(&PrinterControls::eventListener, this);
				closeBtn->connectEventHandler(&PrinterControls::eventListener, this);
			}

			virtual void unActivateListeners()
			{
				closeBtnBig->disconnectEventHandler();
				closeBtn->disconnectEventHandler();
				okBtn->disconnectEventHandler();
			}

			virtual void draw()
			{
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
					drawLayout();									
				gl::popMatrices();

			//	closeBtn->draw();
			//	closeBtnBig->draw();	
			}

			virtual void drawLayout()
			{
				gl::color(ci::Color::hex(0x171521));
				gl::drawSolidRect(ci::Rectf(0.0f, 0.0f, getWindowWidth(), 400.0f));				
				gl::color(ci::Color::white());
				textTools().textFieldDraw(titleText1, ci::Vec2f(150, 95));
				textTools().textFieldDraw(titleText2, ci::Vec2f(157, 158));

				textTools().textFieldDraw(iErrorText, ci::Vec2f(202, 246));
				gl::draw(closeIcon, ci::Vec2f(160, 248));

				gl::color(ci::Color::hex(0x6798ff));
				gl::drawSolidRoundedRect(okButtonArea, 7);	
				gl::color(ci::Color::white());
				textTools().textFieldDraw(changedText, okTextPos);
			}

			virtual void setAlpha(float alpha)
			{

			}		

		private:
			ConfigSettingsRef	configSettings;
			TextItem titleText1, titleText2, iErrorText, changedText;
			ci::gl::Texture closeIcon;

			ci::Rectf okButtonArea, closeButtonArea;
			ci::Vec2f okTextPos;

			SimpleSpriteButtonRef closeBtn, closeBtnBig, okBtn;
		};

		typedef std::shared_ptr<PrinterControls> PrinterControlsRef;
	}
}