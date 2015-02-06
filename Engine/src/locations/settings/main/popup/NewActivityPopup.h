#pragma once

#include "ConfigSettings.h"
#include "gui/CompositeDispatcher.h"
#include "StartNewActivityEvent.h"
#include "CloseActivityEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NewActivityPopup> NewActivityPopupRef;

		class NewActivityPopup: public CompositeDispatcher
		{
		public:	
			NewActivityPopup(ConfigSettingsRef configSett)
				:CompositeDispatcher(),
				configSett(configSett),
				bgColor(Color::hex(0x2f3643)),
				headColor(Color::hex(0x424d5f)),
				titlesColor(ColorA::white()),
				inputFieldColor(Color::hex(0xe5e9f2)),
				btnStartColor(Color::hex(0x6798ff)),
				closeIcon(configSett->getTexture("closePrinterIcon"))	
			{
				mainTitle	= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_ASK_TITLE), true, -10);
				subTitle	= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_DESQR), true, -8);
				title		= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_TITLE));
				begin		= textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_BEGIN));
				closeTitle  = textTools().getTextField(configSett->getTextItem(ConfigTextID::PARTY_CLOSE));	

				float allWidth = closeIcon.getWidth() + 20 + closeTitle.getWidth();
				float startX = 0.5 * (getWindowWidth() - allWidth);

				closeBtnPos = Vec2f(startX, 100);
				CloseActivityEventRef  closeEvent = CloseActivityEventRef(new CloseActivityEvent());
				closeBtn = SimpleButtonRef(new SimpleButton(Rectf(closeBtnPos, closeBtnPos + Vec2f(allWidth, 40)), closeEvent));						
			
				StartNewActivityEventRef  startEvent = StartNewActivityEventRef(new StartNewActivityEvent());
				startBtn = SimpleButtonRef(new SimpleButton(Rectf(750, 932, 962 , 1072), startEvent));						
			}			

			virtual void draw()
			{
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());			
					drawBackgrounds();
					drawTitles();
					drawInputFieldBackground();
					drawCloseBlock();				
				gl::popMatrices();
				CompositeDispatcher::draw();
			}

			void drawBackgrounds()
			{
				gl::color(headColor);
				gl::drawSolidRect(Rectf(0, 0, getWindowWidth(), 717));
				gl::color(bgColor);
				gl::pushMatrices();	
				gl::translate(0, 717);
				gl::drawSolidRect(Rectf(0, 0, getWindowWidth(), 1920 - 717));
				gl::color(Color::white());
				gl::popMatrices();
			}

			void drawTitles()
			{
				gl::color(titlesColor);
				gl::draw(mainTitle, Vec2f(0.5*(getWindowWidth() - mainTitle.getWidth()), 200));
				gl::draw(subTitle, Vec2f(0.5*(getWindowWidth() - subTitle.getWidth()), 435));
				gl::draw(title, Vec2f(0.5*(getWindowWidth() - title.getWidth()), 843));
			}

			void drawInputFieldBackground()
			{				
				gl::color(inputFieldColor);
				gl::drawSolidRoundedRect(Rectf(130, 932, 130 + 828, 932 + 140), 7);
				gl::color(btnStartColor);
				gl::drawSolidRoundedRect(Rectf(752, 932, 962, 1072), 7);	
				gl::drawSolidRect(Rectf(750, 932, 760 , 1072), 7);	

				gl::color(titlesColor);
				gl::draw(begin, Vec2f( 750 + 0.5 * (210 - begin.getWidth()), 932 + 0.5 * (142 - begin.getHeight())));	
			}

			void drawCloseBlock()
			{	
				gl::color(titlesColor);
				gl::draw(closeIcon, closeBtnPos);
				gl::draw(closeTitle, closeBtnPos + Vec2f(20 + closeIcon.getWidth(), - 5));		
			}

			virtual void setAlpha(float  alpha)
			{
				bgColor = Utils::colorAlpha(bgColor, alpha);
				headColor = Utils::colorAlpha(headColor, alpha);
				titlesColor = Utils::colorAlpha(titlesColor, alpha);
				inputFieldColor = Utils::colorAlpha(titlesColor, alpha);
				btnStartColor = Utils::colorAlpha(btnStartColor, alpha);
				CompositeDispatcher::setAlpha(alpha);
			}

			void show(EaseFn eFunc = EaseOutCubic(), float time = 0.7f)
			{
				alpha = 0.0f;
				timeline().apply(&alpha, 0.97f, time, eFunc)
					.updateFn(bind( &NewActivityPopup::alphAnimationUpdate, this))
					.finishFn( bind( &NewActivityPopup::showAnimationFinish, this));
			}

			void hide(EaseFn eFunc = EaseOutCubic(), float time = 0.7f)
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
				closeBtn->addMouseUpListener(&NewActivityPopup::mouseUpFunction, this);	
				startBtn->addMouseUpListener(&NewActivityPopup::mouseUpFunction, this);	
				ShowCompleteSignal();
			}

			void hideAnimationFinish()
			{
				closeBtn->removeMouseUpListener();	
				startBtn->removeMouseUpListener();	
				HideCompleteSignal();
			}

			SignalVoid HideCompleteSignal;
			SignalVoid ShowCompleteSignal;

		protected:
			ConfigSettingsRef configSett;
			ci::ColorA bgColor, headColor, titlesColor, inputFieldColor, btnStartColor;
			ci::Anim<float> alpha;
			ci::gl::Texture subTitle, mainTitle, title, begin, closeIcon, closeTitle;			

			SimpleButtonRef closeBtn, startBtn;
			ci::Vec2f closeBtnPos;
		};
	}
}
