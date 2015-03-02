#pragma once
#include "gui/Sprite.h"

#include "main/Title.h"
#include "main/StartNewActivity.h"
#include "main/StatBlock.h"
#include "main/designBlock/DesignBlock.h"
#include "main/gamesBlock/GamesBlock.h"
#include "main/printerBlock/PrinterBlock.h"
#include "main/Logo.h"
#include "main/CloseBlock.h"
#include "main/popup/NewActivityPopup.h"
#include "GameCheckerEvent.h"

namespace kubik
{
	namespace config
	{
		class MainConfig: public Sprite
		{
		public:
			static const int SHOW_ANIM_COMPLETE = 0; 

			MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings)
				:Sprite(), configSettings(configSettings), gameSettings(gameSettings)
			{			
				setAlpha(1);

				printerBlock	 = PrinterBlockRef(new PrinterBlock(configSettings, ci::Vec2i(0, getWindowHeight() - 170.0f)));			
				logo			 = LogoRef(new Logo(configSettings, ci::Vec2i(835, getWindowHeight() - 170.0f)));
				startNewActivity = StartNewActivityRef(new StartNewActivity(configSettings, ci::Vec2i(96, 137)));	
				newActPopup		 = NewActivityPopupRef(new NewActivityPopup(configSettings));
				statBlock		 = StatBlockRef(new StatBlock(configSettings, ci::Vec2i(100, 235)));
				title			 = TitleRef(new Title(configSettings, ci::Vec2i(100, 60)));	
				closeBlock		 = CloseBlockRef(new CloseBlock(configSettings, ci::Vec2i(916, 66)));				
				designBlock		 = DesignBlockRef(new DesignBlock(configSettings, ci::Vec2i(100, 600.0f - 178.0f)));		
				gamesBlock		 = GamesBlockRef(new GamesBlock(configSettings, gameSettings, ci::Vec2f(100.0f, 600.0f)));				
				
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
				closeBlock->connectEventHandler(&MainConfig::closeHandler, this);	

				startNewActivity->connectEventHandler(&MainConfig::openPopupNewActivity, this);	

				printerBlock->connectEventHandler(&MainConfig::openPrinterControls, this, PrinterBlock::OPEN_EVENT);				
				printerBlock->activateListeners();	

				designBlock->connectEventHandler(&MainConfig::openingDesignLayout,	this, DesignBlock::OPEN_EVENT);	
				designBlock->unlockListeners();
				designBlock->activateListeners();

				gamesBlock->activateListeners();
				gamesBlock->connectEventHandler(&MainConfig::gamesBlockEventsHandler, this);				
			}

			virtual void unActivateListeners()
			{
				closeBlock->disconnectEventHandler();

				gamesBlock->disconnectEventHandler();
				gamesBlock->unActivateListeners();									
		
				printerBlock->disconnectEventHandler(PrinterBlock::OPEN_EVENT);
				startNewActivity->disconnectEventHandler();

				designBlock->disconnectEventHandler(PrinterBlock::OPEN_EVENT);	
				designBlock->lockListeners();
			}

			void closeHandler(EventGUIRef& event)
			{
				logger().log("closeHandler");
			}					

			////////////////////////////////////////////////////////////////////////////
			//
			//				PRINTER CONTROLS
			//
			////////////////////////////////////////////////////////////////////////////

			void openPrinterControls()
			{
				unActivateListeners();
				printerBlock->openControls();				
				logo->animateToUpState();				
				printerBlock->connectEventHandler(&MainConfig::printerControlsOpened, this, PrinterBlock::OPENED);
			}

			void printerControlsOpened()
			{		
				printerBlock->connectEventHandler(&MainConfig::printerHideHandler,      this, PrinterBlock::HIDE_EVENT);
				printerBlock->connectEventHandler(&MainConfig::printerStatResetHandler, this, PrinterBlock::STAT_RESET_EVENT);
			}

			void printerHideHandler()
			{
				hidePrinterControls();
			}

			void printerStatResetHandler()
			{
				console()<<"reset printer data------------------  "<<endl;
				hidePrinterControls();				
				printerBlock->setÑurrentPhotosPrinted(0);
				configSettings->setCurrentPhotosPrinted(0);			
			}

			void hidePrinterControls()
			{				
				printerBlock->disconnectEventHandler(PrinterBlock::HIDE_EVENT);
				printerBlock->disconnectEventHandler(PrinterBlock::STAT_RESET_EVENT);				
				printerBlock->connectEventHandler(&MainConfig::printerControlsHided, this, PrinterBlock::HIDED);

				logo->animateToDownState();
				printerBlock->closeControls();
			}

			void printerControlsHided()
			{
				printerBlock->disconnectEventHandler(PrinterBlock::HIDED);
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
				addChild(newActPopup);	
				newActPopup->connectEventHandler(&MainConfig::popupOpened, this, NewActivityPopup::OPENED);
				newActPopup->show();
			}
			
			void popupOpened()
			{
				console()<<"POPUP :: OPENED "<<endl;
				newActPopup->disconnectEventHandler(NewActivityPopup::OPENED);
				newActPopup->connectEventHandler(&MainConfig::popupHideHandler,this, NewActivityPopup::HIDE_EVENT);
				newActPopup->connectEventHandler(&MainConfig::popupStartNewCompainHandler, this, NewActivityPopup::START_NEW_COMPAIN);
			}

			void popupHideHandler()
			{
				closingPopup();
			}

			void popupStartNewCompainHandler()
			{
				closingPopup();
				
				if (configSettings->getActionName() != newActPopup->getCompainName())
				{
					startNewCampain();					
				}
				else
				{
					console()<<"nothing changed------------------  "<<endl;
				}				
			}

			void startNewCampain()
			{
				// TODO START NEW COMPAIN
				console()<<"start new compain------------------  "<<endl;
				title->setActivityName(newActPopup->getCompainName());
				configSettings->setActionName(newActPopup->getCompainName());

				statBlock->nullValues();
				configSettings->setPlayedCount(0);
				configSettings->setPrintedCount(0);
				configSettings->setPuplishedCount(0);			
			}

			void closingPopup()
			{
				newActPopup->disconnectEventHandler(NewActivityPopup::HIDE_EVENT);
				newActPopup->disconnectEventHandler(NewActivityPopup::START_NEW_COMPAIN);				
				newActPopup->connectEventHandler(&MainConfig::popupClosed, this, NewActivityPopup::HIDED);
				newActPopup->hide();			
			}

			void popupClosed()
			{
				removeChild(newActPopup);	
				newActPopup->disconnectEventHandler(NewActivityPopup::HIDED);
				activateListeners();
			}

			////////////////////////////////////////////////////////////////////////////
			//
			//				DESIGN BLOCK
			//
			////////////////////////////////////////////////////////////////////////////		

			void openingDesignLayout()
			{
				unActivateListeners();
				designBlock->disconnectEventHandler(DesignBlock::OPEN_EVENT);
				designBlock->connectEventHandler(&MainConfig::openedDesignLayout,		this, DesignBlock::OPENED);				
				gamesBlock->hide(EaseOutCubic(), 0.9f);
				designBlock->showDesigns(EaseOutCubic(), 0.9f);				
			}

			void openedDesignLayout()
			{
				activateListeners();
				designBlock->setOpened(true);
				designBlock->disconnectEventHandler(DesignBlock::OPENED);
				designBlock->activateListeners();				

				designBlock->connectEventHandler(&MainConfig::screenSaverStateChanged,  this, DesignBlock::SCREEN_SAVER_STATE);
				designBlock->connectEventHandler(&MainConfig::screenSaverOpenFolder,	this, DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
				designBlock->connectEventHandler(&MainConfig::changedKubikDesign,		this, DesignBlock::CHANGED_DESIGN);
				designBlock->connectEventHandler(&MainConfig::openUserDesignFolder,		this, DesignBlock::OPEN_USE_DESIGN_FOLDER);
				designBlock->connectEventHandler(&MainConfig::hidingDesignLayout,		this, DesignBlock::HIDE);
			}

			void screenSaverStateChanged()
			{
				console()<<"screen saver changed state------------------  "<< designBlock->getScreenSaverValue()<<endl;
			}

			void screenSaverOpenFolder()
			{
				console()<<"screen saver open folder------------------  "<<endl;
			}

			void changedKubikDesign()
			{
				console()<<"changed kubik design------------------  "<< designBlock->getDesignID()<<endl;
				configSettings->setActiveDesignID(designBlock->getDesignID());
			}
	
			void openUserDesignFolder()
			{
				console()<<"open user design folder------------------  "<<endl;
			}

			void hidingDesignLayout()
			{
				console()<<"hidingDesignLayout------------------  "<<endl;
				unActivateListeners();

				designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_STATE);
				designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
				designBlock->disconnectEventHandler(DesignBlock::CHANGED_DESIGN);
				designBlock->disconnectEventHandler(DesignBlock::OPEN_USE_DESIGN_FOLDER);
				designBlock->disconnectEventHandler(DesignBlock::HIDE);
				designBlock->connectEventHandler(&MainConfig::hideDesignLayot,  this, DesignBlock::HIDED);
				designBlock->hideDesigns(EaseOutCubic(), 0.9f);
				gamesBlock->show(EaseOutCubic(), 0.9f);
			}
			
			void hideDesignLayot()
			{	
				designBlock->setOpened(false);
				designBlock->disconnectEventHandler(DesignBlock::HIDED);				
				activateListeners();		
			}

			void closeDesignBlock()
			{
				if(designBlock->getOpened())
				{
					designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_STATE);
					designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
					designBlock->disconnectEventHandler(DesignBlock::CHANGED_DESIGN);
					designBlock->disconnectEventHandler(DesignBlock::OPEN_USE_DESIGN_FOLDER);
					designBlock->disconnectEventHandler(DesignBlock::HIDE);
					designBlock->hideDesigns(EaseOutCubic(), 0.2f);
					gamesBlock->show(EaseOutCubic(), 0.9f);
				}
			}			

			////////////////////////////////////////////////////////////////////////////
			//
			//			GAMES BLOCK	
			//
			////////////////////////////////////////////////////////////////////////////

			void gamesBlockEventsHandler(EventGUIRef& event)
			{
				EventGUI *ev = event.get();
				if(!ev)
					return;

				if(typeid(*event.get()) == typeid(GameConfEvent))
				{
					GameConfEventRef eventref = static_pointer_cast<GameConfEvent>(event);
					console()<<"go to config page------------------  "<<eventref->getGameId()<<endl;
					mouseUpSignal(event);
				}
				else if(typeid(*ev) == typeid(StatisticEvent))
				{
					StatisticEventRef eventref = static_pointer_cast<StatisticEvent>(event);
					console()<<"show statistic------------------  "<<eventref->getGameId()<<endl;
				}
				else if(typeid(*ev) == typeid(GameCheckerEvent))
				{					
					GameCheckerEventRef eventref = static_pointer_cast<GameCheckerEvent>(event);
					gameSettings->setGameActive(eventref->getGameId(), eventref->getValue());
					console()<<"active checker changed value -------------------"<<eventref->getValue()<<"   "<<eventref->getGameId()<<endl;
				}
				else if(typeid(*ev) == typeid(GameShowUrlEvent))
				{					
					GameShowUrlEventRef eventref = static_pointer_cast<GameShowUrlEvent>(event);
					console()<<"show  game url -------------------"<<eventref->getGameId()<<endl;
				}				
			}		

			void setStartupData()
			{
				statBlock->setPlayedTimes(configSettings->getPlayedCount());
				statBlock->setPrintedPhotos(configSettings->getPrintedCount());
				statBlock->setSharedAndEmail(configSettings->getPublishedCount());
				printerBlock->setÑurrentPhotosPrinted(configSettings->getCurrentPhotosPrinted());	
			}

			virtual void draw()
			{
				gl::color(ci::Color::hex(0x0d0917));				
				gl::drawSolidRect(getWindowBounds());
				Sprite::draw();
			}		

			void startAnimation(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				alpha = 0.0f;
				animatePosition = ci::Vec2f(1080.0f, 0.0f);

				Sprite::setAlpha(alpha);	
				setPosition(animatePosition.value());	
				
				timeline().apply(&alpha, 1.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				    .finishFn(bind( &MainConfig::startAnimationFinish, this));

				timeline().apply( &animatePosition, ci::Vec2f::zero(), time, eFunc)
					     .updateFn(bind( &MainConfig::posAnimationUpdate, this));
			}
			
			void alphAnimationUpdate()
			{
				Sprite::setAlpha(alpha);
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

			void hideAnimate(GameId id, ci::EaseFn eFunc, float time)
			{	
				unActivateListeners();	

				ci::Vec2f finPos = ci::Vec2f(-1080.0f + logo->getWidth(), 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &MainConfig::posAnimationUpdate, this));
		
				timeline().apply(&alpha, 0.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				.finishFn( bind( &MainConfig::hideAnimationFinish, this));


				Texture icon = gameSettings->getData().getPurchasedGameInfo(id).getIcons().activeIcon;
				logo->animateToMiniState(icon, eFunc, time);
				closeBlock->animateToMiniState(eFunc, time, finPos);
			}

			void hideAnimationFinish()
			{
				closeBlock->connectEventHandler(&MainConfig::closeHandler, this);				
			}			

			void showAnimate(ci::EaseFn eFunc, float time)
			{
				closeBlock->disconnectEventHandler();
				
				ci::Vec2f finPos = ci::Vec2f::zero();
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
				callback(SHOW_ANIM_COMPLETE);				
				activateListeners();
			}			

		private:
			ConfigSettingsRef	configSettings;
			GameSettingsRef		gameSettings;
			CloseBlockRef		closeBlock;
			TitleRef			title;
			StartNewActivityRef startNewActivity;
			StatBlockRef		statBlock;
			DesignBlockRef		designBlock;
			GamesBlockRef		gamesBlock;
			PrinterBlockRef		printerBlock;
			LogoRef				logo;
			NewActivityPopupRef	popup, newActPopup;
			
			ci::Anim<ci::Vec2f> animatePosition;
			ci::Anim<float> alpha;
		};

		typedef std::shared_ptr<MainConfig> MainConfigRef;
	}
}