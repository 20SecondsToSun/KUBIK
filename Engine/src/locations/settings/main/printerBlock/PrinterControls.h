#pragma once
#include "gui/CompositeDispatcher.h"
#include "ConfigSettings.h"
#include "PrinterControlsHideEvent.h"
#include "PrinterStatResetEvent.h"

namespace kubik
{
	namespace config
	{
		class PrinterControls: public CompositeDispatcher
		{
		public:	
			PrinterControls(ConfigSettingsRef configSettings, ci::Vec2i position)
				:CompositeDispatcher(), configSettings(configSettings),
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
				okBtn = SimpleButtonRef(new SimpleButton(okButtonArea, eventReset));				
				okBtn->setAlpha(0.5f);
				addChild(okBtn);

				PrinterControlsHideEventRef eventHide = PrinterControlsHideEventRef(new PrinterControlsHideEvent());
				closeBtn = SimpleButtonRef(new SimpleButton(415, 40, Vec2f(160, 248), eventHide));				
				closeBtn->setAlpha(0.5f);
				addChild(closeBtn);	

				closeBtnBig = SimpleButtonRef(new SimpleButton(getWindowWidth(), 1920 - 400 - 170, Vec2f(0, -(1920 - 400)), eventHide));				
				closeBtnBig->setAlpha(0.5f);
				addChild(closeBtnBig);	
			}			

			virtual void activateListeners()
			{
				closeBtnBig->addMouseUpListener(&PrinterControls::mouseUpFunction, this);				
				closeBtn->addMouseUpListener(&PrinterControls::mouseUpFunction, this);				
				okBtn->addMouseUpListener(&PrinterControls::mouseUpFunction, this);
			}

			virtual void unActivateListeners()
			{
				closeBtnBig->removeMouseUpListener();
				closeBtn->removeMouseUpListener();
				okBtn->removeMouseUpListener();
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
				gl::color(Color::hex(0x171521));
				gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 400.0f));				
				gl::color(Color::white());
				textTools().textFieldDraw(titleText1, Vec2f(150, 95));
				textTools().textFieldDraw(titleText2, Vec2f(157, 158));

				textTools().textFieldDraw(iErrorText, Vec2f(202, 242));
				gl::draw(closeIcon, Vec2f(160, 248));

				gl::color(Color::hex(0x6798ff));
				gl::drawSolidRoundedRect(okButtonArea, 7);	
				gl::color(Color::white());
				textTools().textFieldDraw(changedText, okTextPos);
			}

			virtual void setAlpha(float alpha)
			{

			}

		private:
			ConfigSettingsRef	configSettings;
			TextItem titleText1, titleText2, iErrorText, changedText;
			Texture closeIcon;

			Rectf okButtonArea, closeButtonArea;
			Vec2f okTextPos;

			SimpleButtonRef closeBtn, closeBtnBig, okBtn;
		};

		typedef std::shared_ptr<PrinterControls> PrinterControlsRef;
	}
}