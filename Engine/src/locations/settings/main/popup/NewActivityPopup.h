#pragma once

#include "ConfigSettings.h"
#include "gui/CompositeDispatcher.h"
#include "StartNewActivityEvent.h"
#include "CloseActivityEvent.h"
#include "VirtualKeyboard.h"
#include "gui/SimpleSpriteButton.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NewActivityPopup> NewActivityPopupRef;

		class NewActivityPopup: public Sprite
		{
		public:
			static const int OPENED = 1;
			static const int HIDE_EVENT = 2;
			static const int START_NEW_COMPAIN = 3;			
			static const int HIDED = 4;

			NewActivityPopup(ConfigSettingsRef configSett)
				:Sprite(),
				configSett(configSett),
				bgColor(ci::Color::hex(0x2f3643)),
				headColor(ci::Color::hex(0x424d5f)),
				titlesColor(ci::ColorA::white()),
				inputFieldColor(ci::Color::hex(0xe5e9f2)),
				btnStartColor(ci::Color::hex(0x6798ff)),
				closeIcon(configSett->getTexture("closePrinterIcon")),
				inputField(configSett->getTexture("inputField"))	
			{
				mainTitle	= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_ASK_TITLE), true, -10);
				subTitle	= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_DESQR), true, -8);
				title		= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_TITLE));
				begin		= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_BEGIN));
				closeTitle  = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_CLOSE));	

				float allWidth = closeIcon.getWidth() + 20 + closeTitle.getWidth();
				float startX = 0.5 * (getWindowWidth() - allWidth);

				closeBtnPos = ci::Vec2f(startX, 100);
				CloseActivityEventRef  closeEvent = CloseActivityEventRef(new CloseActivityEvent());
				closeBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(ci::Rectf(closeBtnPos, closeBtnPos + ci::Vec2f(allWidth, 40)), closeEvent));						
			

				yPositionInputField = 200;//932.0f;
				inputFieldPos = Vec2f(0.5f * (1080.0f - inputField.getWidth()), yPositionInputField);
				StartNewActivityEventRef  startEvent = StartNewActivityEventRef(new StartNewActivityEvent());
				startBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(ci::Rectf(750, yPositionInputField, 962 , yPositionInputField + 140), startEvent));				
			}			

			virtual void draw()
			{
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());			
					drawBackgrounds();
					//drawTitles();
					drawInputFieldBackground();
					drawCloseBlock();	
					touchKeyboard().draw();
				gl::popMatrices();
				Sprite::draw();
			}

			void drawBackgrounds()
			{
				gl::color(headColor);
				gl::drawSolidRect(ci::Rectf(0, 0, getWindowWidth(), 717));
				gl::color(bgColor);
				gl::pushMatrices();	
				gl::translate(0, 717);
				gl::drawSolidRect(ci::Rectf(0, 0, getWindowWidth(), 1920 - 717));
				gl::color(ci::Color::white());
				gl::popMatrices();
			}

			void drawTitles()
			{
				gl::color(titlesColor);
				gl::draw(mainTitle, ci::Vec2f(0.5*(getWindowWidth() - mainTitle.getWidth()), 200));
				gl::draw(subTitle, ci::Vec2f(0.5*(getWindowWidth() - subTitle.getWidth()), 435));
				gl::draw(title, ci::Vec2f(0.5*(getWindowWidth() - title.getWidth()), 843));
			}

			void drawInputFieldBackground()
			{				
				/*
				gl::drawSolidRoundedRect(ci::Rectf(130, 932, 130 + 828, 932 + 140), 7);
				gl::color(btnStartColor);
				gl::drawSolidRoundedRect(ci::Rectf(752.0, 932.0, 962.0, 1072.0), 7);	
				gl::drawSolidRect(ci::Rectf(750.0f, 932.0, 760.0, 1072.0), 7);	*/
				gl::color(inputFieldColor);
				gl::draw(inputField, inputFieldPos);
				gl::color(titlesColor);
				gl::draw(begin, ci::Vec2f( 750.0 + 0.5 * (210 - begin.getWidth()), yPositionInputField + 0.5 * (142 - begin.getHeight())));	
			}

			void drawCloseBlock()
			{	
				gl::color(titlesColor);
				gl::draw(closeIcon, closeBtnPos);
				gl::draw(closeTitle, closeBtnPos + Vec2f(20 + closeIcon.getWidth(), -1));		
			}

			virtual void setAlpha(float  alpha)
			{
				bgColor = Utils::colorAlpha(bgColor, alpha);
				headColor = Utils::colorAlpha(headColor, alpha);
				titlesColor = Utils::colorAlpha(titlesColor, alpha);
				inputFieldColor = Utils::colorAlpha(titlesColor, alpha);
				btnStartColor = Utils::colorAlpha(btnStartColor, alpha);
				Sprite::setAlpha(alpha);
			}

			void show(EaseFn eFunc = EaseOutCubic(), float time = 0.4f)
			{
				alpha = 0.0f;
				timeline().apply(&alpha, 0.97f, time, eFunc)
					.updateFn(bind( &NewActivityPopup::alphAnimationUpdate, this))
					.finishFn( bind( &NewActivityPopup::showAnimationFinish, this));

				touchKeyboard().setInputField(inputFieldPos.x, inputFieldPos.y, inputFieldPos.x + 621, inputFieldPos.y + 139);
			}

			void hide(EaseFn eFunc = EaseOutCubic(), float time = 0.3f)
			{				
				timeline().apply(&alpha, 0.0f, time, eFunc)
					.updateFn(bind( &NewActivityPopup::alphAnimationUpdate, this))
					.finishFn( bind( &NewActivityPopup::hideAnimationFinish, this));
			}

			void alphAnimationUpdate()
			{
				setAlpha(alpha.value());
			}

			void showAnimationFinish()
			{
				if(eventHandlerDic[OPENED])
				{
					eventHandlerDic[OPENED]();
					closeBtn->connectEventHandler(&NewActivityPopup::closeHandler, this);
					startBtn->connectEventHandler(&NewActivityPopup::newCompainHandler, this);
				}	

				
				//touchKeyboard().setEraseButtonVisible(false);
				touchKeyboard().setOriginPoint(Vec2f(0, 0));
				touchKeyboard().connectKeyboard();
				touchKeyboard().connectEventHandler(&NewActivityPopup::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);
				//touchKeyboard().connectEventHandler(&KeyboardApp::keyTouchHandler,	 this, VirtualKeyboard::KEY_TOUCH);	
			}

			void inputTouchHandler()
			{
				console()<<"inputTouchHandler"<<endl;
				touchKeyboard().show(ci::Vec2f(30, 800), Vec2f(30, 460), 0.7);
			}

			void closeHandler(EventGUIRef& event)
			{
				if(eventHandlerDic[HIDE_EVENT])
				{
					disconnect();
					eventHandlerDic[HIDE_EVENT]();
				}
			}

			void newCompainHandler(EventGUIRef& event)
			{
				if(eventHandlerDic[START_NEW_COMPAIN])
				{
					disconnect();
					eventHandlerDic[START_NEW_COMPAIN]();					
				}
			}

			void disconnect()
			{
				closeBtn->disconnectEventHandler();
				startBtn->disconnectEventHandler();	

				touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);	
				//touchKeyboard().connectEventHandler(&NewActivityPopup::inputTouchHandler, this, VirtualKeyboard::HIDED);
				touchKeyboard().disconnectKeyboard();
				touchKeyboard().hide(ci::Vec2f(30, 1200), 0.3);
			}

			void hideAnimationFinish()
			{
				callback(HIDED);				
			}
	
		protected:
			ConfigSettingsRef configSett;
			ci::ColorA bgColor, headColor, titlesColor, inputFieldColor, btnStartColor;
			ci::Anim<float> alpha;
			ci::gl::Texture subTitle, mainTitle, title, begin, closeIcon, closeTitle, inputField;			

			SimpleSpriteButtonRef closeBtn, startBtn;
			ci::Vec2f closeBtnPos, inputFieldPos;

			float yPositionInputField;
		};
	}
}
