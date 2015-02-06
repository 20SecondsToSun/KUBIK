#pragma once

#include "gui/CompositeDispatcher.h"
#include "ConfigSettings.h"


namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class MainPopup> MainPopupRef;

		class MainPopup: public CompositeDispatcher
		{
		public:	
			MainPopup(ConfigSettingsRef configSett)
				:CompositeDispatcher(),
				bgColor(Color::black())
			{
				closeBlock = CloseBlockRef(new CloseBlock(configSett, Vec2i(66, 66)));
				closeBlock->addMouseUpListener(&MainPopup::mouseUpFunction, this);
				
				addChild(closeBlock);
			}			

			virtual void draw()
			{
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
					gl::color(bgColor);
					gl::drawSolidRect(getWindowBounds());
					gl::color(Color::white());
				gl::popMatrices();
				CompositeDispatcher::draw();
			}

			void show(EaseFn eFunc = EaseOutCubic(), float time = 0.7f)
			{
				alpha = 0.0f;
				timeline().apply(&alpha, 0.85f, time, eFunc)
					.updateFn(bind( &MainPopup::alphAnimationUpdate, this))
				.finishFn( bind( &MainPopup::showAnimationFinish, this));
			}
			
			void hide(EaseFn eFunc = EaseOutCubic(), float time = 0.7f)
			{				
				timeline().apply(&alpha, 0.0f, time, eFunc)
					.updateFn(bind( &MainPopup::alphAnimationUpdate, this))
				.finishFn( bind( &MainPopup::hideAnimationFinish, this));
			}

			void alphAnimationUpdate()
			{
				setAlpha(alpha.value());
			}

			void showAnimationFinish()
			{

			}

			void hideAnimationFinish()
			{
				HideCompleteSignal();
			}

			virtual void setAlpha(float  alpha)
			{
				bgColor = Utils::colorAlpha(bgColor, alpha);
				closeBlock->setAlpha(alpha);
				CompositeDispatcher::setAlpha(alpha);
			}

			SignalVoid HideCompleteSignal;

		protected:
			ColorA bgColor;
			Anim<float> alpha;
			CloseBlockRef closeBlock;			
		};
	}
}
