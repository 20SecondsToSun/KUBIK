#pragma once
#include "gui/CompositeDispatcher.h"

#include "main/Title.h"
#include "main/StartNewActivity.h"
#include "main/StatBlock.h"
#include "main/designBlock/DesignBlock.h"
#include "main/gamesBlock/GamesBlock.h"
#include "main/printerBlock/PrinterBlock.h"
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
				designBlock		 = DesignBlockRef(new DesignBlock(configSettings, Vec2i(100, 424)));	
				gamesBlock		 = GamesBlockRef(new GamesBlock(configSettings, gameSettings, Vec2f(100.0f, 600.0f)));
				printerBlock	 = PrinterBlockRef(new PrinterBlock(configSettings, Vec2i(0, getWindowHeight() - 170.0f)));			
				logo			 = LogoRef(new Logo(configSettings, Vec2i(835, getWindowHeight() - 170.0f)));

				newActPopup		 = NewActivityPopupRef(new NewActivityPopup(configSettings));
			//	changeCatridgePopup		 = ChangeCatridgePopupRef(new ChangeCatridgePopup(configSettings));
		
				addChild(closeBlock);
				addChild(title);
				addChild(startNewActivity);				
				addChild(statBlock);				
				addChild(designBlock);				
				addChild(gamesBlock);
				addChild(printerBlock);	
				addChild(logo);	
			}

			virtual void activateListeners()
			{
				closeBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);
				gamesBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);	
				startNewActivity->addMouseUpListener(&MainConfig::openPopupNewActivity, this);	
				printerBlock->addMouseUpListener(&MainConfig::openPrinterControls, this);	
				designBlock->addMouseUpListener(&MainConfig::designBlockEventHandler, this);	
				CompositeDispatcher::activateListeners();				
			}

			void openPrinterControls(EventGUIRef& event)
			{
				unActivateListeners();
				printerBlock->openControls();
				printerBlock->addMouseUpListener(&MainConfig::printerButtonsHandler, this);	
				logo->animateToUpState();
			}

			void printerButtonsHandler(EventGUIRef& event)
			{
				logger().log("printer handler:::");
				EventGUI *ev = event.get();		

				if(typeid(*ev) == typeid(PrinterControlsHideEvent))
				{
					printerBlock->removeMouseUpListener();
					logo->animateToDownState();
					printerBlock->closeControls();
					connect_once(printerBlock->HideCompleteSignal, bind(&MainConfig::printerControlsHide, this));
				}
				else if(typeid(*ev) == typeid(PrinterStatResetEvent))
				{
					printerBlock->removeMouseUpListener();
					logo->animateToDownState();
					printerBlock->closeControls();
					connect_once(printerBlock->HideCompleteSignal, bind(&MainConfig::printerControlsHide, this));
				}
			}

			void printerControlsHide()
			{
				activateListeners();
			}

			////////////////////////////////////////////////////////////////////////////
			//
			//				POPUP
			//
			////////////////////////////////////////////////////////////////////////////

			void openPopupNewActivity(EventGUIRef& event)
			{
				unActivateListeners();		
							
				connect_once(newActPopup->ShowCompleteSignal, bind(&MainConfig::popupOpened, this));
				addChild(newActPopup);
				newActPopup->show();
			}

			void popupOpened()
			{
				newActPopup->addMouseUpListener(&MainConfig::popupHandler, this);	
				newActPopup->ShowCompleteSignal.disconnect_all_slots();	
				connect_once(newActPopup->HideCompleteSignal, bind(&MainConfig::popupClosed, this));
			}

			void popupClosed()
			{
				activateListeners();
				newActPopup->removeMouseUpListener();
				newActPopup->HideCompleteSignal.disconnect_all_slots();	
				removeChild(newActPopup);
			}

			void popupHandler(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if(typeid(*ev) == typeid(CloseActivityEvent))
				{
					newActPopup->hide();	
				}
				else if(typeid(*ev) == typeid(StartNewActivityEvent))
				{
					newActPopup->hide();	
				}
			}

			////////////////////////////////////////////////////////////////////////////
			//
			//				
			//
			////////////////////////////////////////////////////////////////////////////


			void designBlockEventHandler(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
			
				if(typeid(*ev) == typeid(CloseDesignLayoutEvent))
				{
					unActivateListeners();	
					connect_once(gamesBlock->showAnimCompleteSig, bind(&MainConfig::designBlockAnimHideFin, this));
					gamesBlock->show(EaseOutCubic(), 0.9f);					
				}
				else
				{
					unActivateListeners();					
					connect_once(gamesBlock->hideAnimCompleteSig, bind(&MainConfig::designBlockAnimShowFin, this));
					gamesBlock->hide(EaseOutCubic(), 0.9f);
					designBlock->showDesigns(EaseOutCubic(), 0.9f);
				}
			}

			void designBlockAnimShowFin()
			{				
				activateListeners();
			}

			void designBlockAnimHideFin()
			{
				designBlock->hideDesigns(EaseOutCubic(), 0.9f);
				activateListeners();
			}

			virtual void unActivateListeners()
			{
				closeBlock->removeMouseUpListener();
				gamesBlock->removeMouseUpListener();
				gamesBlock->unActivateListeners();
				startNewActivity->removeMouseUpListener();							
				printerBlock->removeMouseUpListener();					
				designBlock->removeMouseUpListener();					
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
			DesignBlockRef		designBlock;
			GamesBlockRef		gamesBlock;
			PrinterBlockRef		printerBlock;
			LogoRef				logo;
			NewActivityPopupRef	 popup, newActPopup;

			list<IDispatcherRef>  components;
			Anim<Vec2f> animatePosition;
			Anim<float> alpha;
		};

		typedef std::shared_ptr<MainConfig> MainConfigRef;
	}
}