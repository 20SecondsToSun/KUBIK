#pragma once
#include "IDispatcher.h"

#include "main/Title.h"
#include "main/StartNewActivity.h"
#include "main/StatBlock.h"
#include "main/MenuBlock.h"
#include "main/ScreenSaverBlock.h"
#include "main/gamesBlock/GamesBlock.h"
#include "main/PrinterBlock.h"
#include "main/Logo.h"
#include "main/CloseBlock.h"

namespace kubik
{
	namespace config
	{
		class MainConfig: public IDispatcher
		{
		public:	
			MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings)
				:alpha(1)
			{
				this->configSettings = configSettings;
			
				closeBlock		 = CloseBlockRef(new CloseBlock(configSettings, Vec2i(916, 66)));
				title			 = TitleRef(new Title(configSettings, Vec2i(100, 60)));				
				startNewActivity = StartNewActivityRef(new StartNewActivity(configSettings, Vec2i(96, 137)));																	

				//connect_once(startNewActivity->startNewActivitySignal, bind(&ConfigScreen::startNewActivityHandler, this, std::placeholders::_1));
				//connect_once(startNewActivity->cancelNewActivityTrySignal, bind(&ConfigScreen::, this, std::placeholders::_1));
				//connect_once(startNewActivity->tryToStartNewActivitySignal, bind(&ConfigScreen::, this, std::placeholders::_1));

				statBlock = StatBlockRef(new StatBlock(configSettings, Vec2i(100, 235)));
				menuBlock = MenuBlockRef(new MenuBlock(configSettings, Vec2i(100, 424)));	

				//connect_once(menuBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				screenSaverBlock = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, Vec2i(533, 424)));				
				//connect_once(screenSaverBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				gamesBlock = GamesBlockRef(new GamesBlock(configSettings, gameSettings, Vec2f(100.0f, 600.0f)));
				printerBlock = PrinterBlockRef(new PrinterBlock(configSettings, Vec2i(0, getWindowHeight() - 170.0f)));			
				//connect_once(printerBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				logo = LogoRef(new Logo(configSettings, Vec2i(835, getWindowHeight() - 170.0f)));
				
				components.push_back(startNewActivity);
				components.push_back(statBlock);
				components.push_back(menuBlock);
				components.push_back(closeBlock);
				components.push_back(screenSaverBlock);
				components.push_back(gamesBlock);
				components.push_back(printerBlock);
				components.push_back(logo);
				components.push_back(title);			
			}

			virtual void activateListeners()
			{
				closeBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);
				gamesBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);	

				for (auto comp : components)
					comp->activateListeners();
			}

			virtual void unActivateListeners()
			{
				closeBlock->removeMouseUpListener();
				gamesBlock->removeMouseUpListener();	

				for (auto comp : components)
					comp->unActivateListeners();
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

				gl::pushMatrices();
				gl::translate(animatePosition);	
				for (auto comp : components)
					comp->draw();
				gl::popMatrices();
			}	

			void setPosition(Vec2i position)
			{
				this->position = position;
			}

			void hideAnimate(EaseFn eFunc, float time)
			{	
				//removeMouseUpListener();
				unActivateListeners();	

				Vec2f finPos = Vec2f(-1080.0f + logo->getWidth(), 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc);
					//	.finishFn( bind( &MainConfig::hideAnimationFinish, this));

				timeline().apply(&alpha, 0.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				.finishFn( bind( &MainConfig::hideAnimationFinish, this));

				logo->animateToMiniState(eFunc, time);
				closeBlock->animateToMiniState(eFunc, time, finPos);
			}

			void showAnimate(EaseFn eFunc, float time)
			{
				unActivateListeners();	

				Vec2f finPos = Vec2f(0.0f, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc);

				timeline().apply(&alpha, 1.0f, time, eFunc)
					.updateFn(bind( &MainConfig::alphAnimationUpdate, this))
				.finishFn( bind( &MainConfig::hideAnimationFinish, this));

				logo->animateToMaxState(eFunc, time);
				closeBlock->animateToMaxState(eFunc, time);
			}			
			
			void alphAnimationUpdate()
			{
				statBlock->setAlpha(alpha);	
				startNewActivity->setAlpha(alpha);
				title->setAlpha(alpha);
				printerBlock->setAlpha(alpha);
				screenSaverBlock->setAlpha(alpha);	
				menuBlock->setAlpha(alpha);
				gamesBlock->setAlpha(alpha);
			}

			void hideAnimationFinish()
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

			list<IDispatcherRef>  components;
			Anim<Vec2f> animatePosition;
			Anim<float> alpha;
		};

		typedef std::shared_ptr<MainConfig> MainConfigRef;
	}
}