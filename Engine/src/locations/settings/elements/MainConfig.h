#pragma once
#include "IDispatcher.h"
#include "IDrawable.h"

#include "elements/Title.h"
#include "elements/StartNewActivity.h"
#include "elements/StatBlock.h"
#include "elements/MenuBlock.h"
#include "elements/ScreenSaverBlock.h"
#include "elements/gamesBlock/GamesBlock.h"
#include "elements/PrinterBlock.h"
#include "elements/Logo.h"
#include "elements/CloseBlock.h"

namespace kubik
{
	namespace config
	{
		class MainConfig: public IDispatcher
		{
		public:	
			MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings)
			{
				this->configSettings = configSettings;
			
				Font fontBtn			= configSettings->getFont("helvetica20");
				Font introLight44		= configSettings->getFont("introLight44");
				Font helveticaLight22	= configSettings->getFont("helveticaLight22");
				Font helveticaLight24	= configSettings->getFont("helveticaLight24");
				Font introBold110		= configSettings->getFont("introBold110");
				Font introBold72		= configSettings->getFont("introBold72");
			
				closeBlock		 = CloseBlockRef(new CloseBlock(Vec2i(916, 66), configSettings->getTexture("closeImg")));
				title			 = TitleRef(new Title(Vec2i(100, 60), "Promo", introLight44));
				
				startNewActivity = StartNewActivityRef(new StartNewActivity(Vec2i(96, 137),
																			configSettings->getTexture("iconStartNew"),
																			helveticaLight24));

				//connect_once(startNewActivity->startNewActivitySignal, bind(&ConfigScreen::startNewActivityHandler, this, std::placeholders::_1));
				//connect_once(startNewActivity->cancelNewActivityTrySignal, bind(&ConfigScreen::, this, std::placeholders::_1));
				//connect_once(startNewActivity->tryToStartNewActivitySignal, bind(&ConfigScreen::, this, std::placeholders::_1));


				statBlock = StatBlockRef(new StatBlock());
				statBlock->setPosition(Vec2i(100, 235));
				statBlock->setTitleFont(helveticaLight22);
				statBlock->setNumsFont(introBold110);

				menuBlock = MenuBlockRef(new MenuBlock());
				menuBlock->setPosition(Vec2i(100, 424));
				menuBlock->setTitleFont(introLight44);
				menuBlock->setSubTitleFont(helveticaLight22);
				menuBlock->setIcon(configSettings->getTexture("menuIcon"));
				menuBlock->createBtn();
				//connect_once(menuBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				screenSaverBlock = ScreenSaverBlockRef(new ScreenSaverBlock());
				screenSaverBlock->setPosition(Vec2i(533, 424));
				screenSaverBlock->setTitleFont(introLight44);
				screenSaverBlock->setSubTitleFont(helveticaLight22);
				screenSaverBlock->setIcon(configSettings->getTexture("ssIcon"));
				screenSaverBlock->createBtn();
				//connect_once(screenSaverBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				gamesBlock = GamesBlockRef(new GamesBlock(configSettings, gameSettings));
				gamesBlock->setPosition(Vec2f(1080.0f, 10.0f));

				printerBlock = PrinterBlockRef(new PrinterBlock());
				printerBlock->setPosition(Vec2i(0, getWindowHeight() - 170.0f));
				printerBlock->setNumsFont(introBold72);
				printerBlock->setHintFont(helveticaLight24);
				printerBlock->setChangeBtnFont(helveticaLight24);
				printerBlock->setChangeBtnIcon(configSettings->getTexture("catridgeIcon"));
				printerBlock->setMaxPhotosToPrint(configSettings->getData().maxPhotosToPrint);
				printerBlock->createBtn();
				//connect_once(printerBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

				logo = LogoRef(new Logo(Vec2i(835, getWindowHeight() - 170.0f), configSettings->getTexture("logoIcon")));
				
				components.clear();				
				components.push_back(startNewActivity);
				components.push_back(statBlock);
				components.push_back(menuBlock);
				components.push_back(closeBlock);
				components.push_back(screenSaverBlock);
				//components.push_back(gamesBlock);
				components.push_back(printerBlock);
				components.push_back(logo);
				components.push_back(title);

				closeBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);
				gamesBlock->addMouseUpListener(&MainConfig::mouseUpFunction, this);	
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
				gl::color(Color::white());
				gl::color(Color::hex(0x0d0917));
				
				gl::drawSolidRect(getWindowBounds());

				for (auto comp : components)
					comp->draw();	
			}

			void setPosition(Vec2i position)
			{
				this->position = position;
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

			list<IDrawableRef>  components;
		};

		typedef std::shared_ptr<MainConfig> MainConfigRef;
	}
}