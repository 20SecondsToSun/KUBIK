#pragma once
#include "gui/CompositeDispatcher.h"

#include "main/Title.h"
#include "main/StartNewActivity.h"
#include "main/StatBlock.h"
#include "main/MenuBlock.h"
#include "main/ScreenSaverBlock.h"
#include "main/gamesBlock/GamesBlock.h"
#include "main/PrinterBlock.h"
#include "main/Logo.h"
#include "main/CloseBlock.h"
#include "main/popup/NewActivityPopup.h"

namespace kubik
{
	namespace config
	{
		class MainConfig: public CompositeDispatcher
		{
		public:	
			MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings)
				:CompositeDispatcher(),
				configSettings(configSettings)
			{			
				setAlpha(1);

				closeBlock		 = CloseBlockRef(new CloseBlock(configSettings, Vec2i(916, 66)));
				title			 = TitleRef(new Title(configSettings, Vec2i(100, 60)));				
				startNewActivity = StartNewActivityRef(new StartNewActivity(configSettings, Vec2i(96, 137)));		
				statBlock		 = StatBlockRef(new StatBlock(configSettings, Vec2i(100, 235)));
				menuBlock		 = MenuBlockRef(new MenuBlock(configSettings, Vec2i(100, 424)));	
				screenSaverBlock = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, Vec2i(533, 424)));				
				gamesBlock		 = GamesBlockRef(new GamesBlock(configSettings, gameSettings, Vec2f(100.0f, 600.0f)));
				printerBlock	 = PrinterBlockRef(new PrinterBlock(configSettings, Vec2i(0, getWindowHeight() - 170.0f)));			
				logo			 = LogoRef(new Logo(configSettings, Vec2i(835, getWindowHeight() - 170.0f)));

				newActPopup				 = NewActivityPopupRef(new NewActivityPopup(configSettings));
			//	changeCatridgePopup		 = ChangeCatridgePopupRef(new ChangeCatridgePopup(configSettings));
				//newActPopup		 = NewActivityPopupRef(new NewActivityPopup(configSettings));

				addChild(closeBlock);
				addChild(title);
				addChild(startNewActivity);				
				addChild(statBlock);				
				addChild(menuBlock);				
				addChild(screenSaverBlock);	
				addChild(gamesBlock);
				addChild(printerBlock);	
				addChild(logo);	
			}

			virtual void activateListeners()
			{
				closeBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);
				gamesBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);	
				startNewActivity->addMouseUpListener(&MainConfig::openPopupNewActivity, this);	
				CompositeDispatcher::activateListeners();				
			}

			void openPopupNewActivity(EventGUIRef& event)
			{
				unActivateListeners();
				popup = newActPopup;
				popup->addMouseUpListener(&MainConfig::popupHandler, this);
				connect_once(popup->HideCompleteSignal, bind(&MainConfig::popupClosed, this));
				addChild(popup);

				popup->show();
			}

			void popupClosed()
			{
				activateListeners();
				popup->removeMouseUpListener();
				popup->HideCompleteSignal.disconnect_all_slots();	
				removeChild(popup);
			}

			void popupHandler(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if(typeid(*ev) == typeid(CloseConfigEvent))
				{
					popup->hide();	
				}
			}

			virtual void unActivateListeners()
			{
				closeBlock->removeMouseUpListener();
				gamesBlock->removeMouseUpListener();
				gamesBlock->unActivateListeners();
				startNewActivity->removeMouseUpListener();							
			}

			void setStartupData()
			{
				statBlock->setPlayedTimes(configSettings->getData().playedCount);
				statBlock->setPrintedPhotos(configSettings->getData().printedCount);
				statBlock->setSharedAndEmail(configSettings->getData().puplishedCount);
				printerBlock->setÑurrentPhotosPrinted(configSettings->getData().currentPhotosPrinted);	
			}

			virtual void draw()
			{
				gl::color(Color::hex(0x0d0917));				
				gl::drawSolidRect(getWindowBounds());
				CompositeDispatcher::draw();
			}		

			void startAnimation(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				alpha = 0.0f;
				animatePosition = Vec2f(1080.0f, 0.0f);

				CompositeDispatcher::setAlpha(alpha);	
				setPosition(animatePosition.value());	
				
				timeline().apply(&alpha, 1.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				    .finishFn(bind( &MainConfig::startAnimationFinish, this));

				timeline().apply( &animatePosition, Vec2f::zero(), time, eFunc)
					     .updateFn(bind( &MainConfig::posAnimationUpdate, this));
			}
			
			void alphAnimationUpdate()
			{
				CompositeDispatcher::setAlpha(alpha);
				closeBlock->setAlpha(1);
			}

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

			void startAnimationFinish()
			{
				activateListeners();
			}

			void hideAnimate(EaseFn eFunc, float time)
			{	
				unActivateListeners();	

				Vec2f finPos = Vec2f(-1080.0f + logo->getWidth(), 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &MainConfig::posAnimationUpdate, this));
		
				timeline().apply(&alpha, 0.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				.finishFn( bind( &MainConfig::hideAnimationFinish, this));

				logo->animateToMiniState(eFunc, time);
				closeBlock->animateToMiniState(eFunc, time, finPos);
			}

			void hideAnimationFinish()
			{
				closeBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);				
			}			

			void showAnimate(EaseFn eFunc, float time)
			{
				closeBlock->removeMouseUpListener();
				
				Vec2f finPos = Vec2f(0.0f, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &MainConfig::posAnimationUpdate, this));

				timeline().apply(&alpha, 1.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				.finishFn( bind( &MainConfig::showAnimationFinish, this));

				logo->animateToMaxState(eFunc, time);
				closeBlock->animateToMaxState(eFunc, time);
			}	

			void showAnimationFinish()
			{
				activateListeners();
			}			

		private:
			ConfigSettingsRef	configSettings;
			CloseBlockRef		closeBlock;
			TitleRef			title;
			StartNewActivityRef startNewActivity;
			StatBlockRef		statBlock;
			MenuBlockRef		menuBlock;
			ScreenSaverBlockRef screenSaverBlock;
			GamesBlockRef		gamesBlock;
			PrinterBlockRef		printerBlock;
			LogoRef				logo;
			MainPopupRef		popup, newActPopup;

			list<IDispatcherRef>  components;
			Anim<Vec2f> animatePosition;
			Anim<float> alpha;
		};

		typedef std::shared_ptr<MainConfig> MainConfigRef;
	}
}