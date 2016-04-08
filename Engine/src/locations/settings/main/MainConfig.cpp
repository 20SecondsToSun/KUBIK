#include "MainConfig.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

MainConfig::MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings, ScreenSaverSettingsRef ssSettings)
	:Sprite(),
	configSettings(configSettings),
	gameSettings(gameSettings),
	ssSettings(ssSettings),
	gamesBlockEnabled(true)
{
	setAlpha(1);

	printerBlock	 = PrinterBlockRef(new PrinterBlock(configSettings, ci::Vec2i(0, getWindowHeight() - 170.0f)));
	logo			 = LogoRef(new Logo(configSettings, ci::Vec2i(835, getWindowHeight() - 170.0f)));
	startNewActivity = StartNewActivityRef(new StartNewActivity(configSettings, ci::Vec2i(96, 137)));
	newActPopup		 = NewActivityPopupRef(new NewActivityPopup(configSettings));
	statBlock		 = StatBlockRef(new StatBlock(configSettings, ci::Vec2i(100, 235)));
	title			 = TitleRef(new Title(configSettings, ci::Vec2i(100, 60)));
	closeBlock		 = CloseBlockRef(new CloseBlock(configSettings, ci::Vec2i(916, 66)));
	designBlock		 = DesignBlockRef(new DesignBlock(configSettings, ssSettings, ci::Vec2i(100, 600.0f - 178.0f)));
	gamesBlock		 = GamesBlockRef(new GamesBlock(configSettings, gameSettings, ci::Vec2f(100.0f, 600.0f)));

	addChild(designBlock);
	addChild(gamesBlock);
	
	addChild(title);
	addChild(startNewActivity);
	
	addChild(statBlock);
	
	addChild(printerBlock);
	addChild(logo);
	addChild(closeBlock);
}

void MainConfig::activateListeners()
{
	closeBlock->connectEventHandler(&MainConfig::closeHandler, this);

	startNewActivity->connectEventHandler(&MainConfig::openPopupNewActivity, this);

	printerBlock->connectEventHandler(&MainConfig::openPrinterControls, this, PrinterBlock::OPEN_EVENT);
	printerBlock->activateListeners();

	designBlock->connectEventHandler(&MainConfig::openingDesignLayout, this, DesignBlock::OPEN_EVENT);
	designBlock->connectEventHandler(&MainConfig::hidingDesignLayout, this, DesignBlock::CLOSE_EVENT);
	designBlock->unlockListeners();
	designBlock->activateListeners();	

	if(gamesBlockEnabled)
	{
		gamesBlock->activateListeners();
		gamesBlock->connectEventHandler(&MainConfig::gamesBlockEventsHandler, this);
	}
}

void MainConfig::unActivateListeners()
{
	closeBlock->disconnectEventHandler();

	gamesBlock->disconnectEventHandler();
	gamesBlock->unActivateListeners();

	printerBlock->disconnectEventHandler(PrinterBlock::OPEN_EVENT);
	printerBlock->unActivateListeners();

	startNewActivity->disconnectEventHandler();

	designBlock->disconnectEventHandler(PrinterBlock::OPEN_EVENT);
	designBlock->disconnectEventHandler(DesignBlock::CLOSE_EVENT);
	designBlock->lockListeners();
}

void MainConfig::killAll()
{
	unActivateListeners();
	designBlock->unActivateListeners();
	designBlock->disconnectEventHandler(DesignBlock::OPENED);
	designBlock->disconnectEventHandler();
}

void MainConfig::closeHandler(EventGUIRef& event)
{
	logger().log("closeHandler");
}

////////////////////////////////////////////////////////////////////////////
//
//				PRINTER CONTROLS
//
////////////////////////////////////////////////////////////////////////////

void MainConfig::openPrinterControls()
{
	unActivateListeners();
	printerBlock->openControls();
	logo->animateToUpState();
	printerBlock->connectEventHandler(&MainConfig::printerControlsOpened, this, PrinterBlock::OPENED);
}

void MainConfig::printerControlsOpened()
{
	printerBlock->connectEventHandler(&MainConfig::printerHideHandler, this, PrinterBlock::HIDE_EVENT);
	printerBlock->connectEventHandler(&MainConfig::printerStatResetHandler, this, PrinterBlock::STAT_RESET_EVENT);
}

void MainConfig::printerHideHandler()
{
	hidePrinterControls();
}

void MainConfig::printerStatResetHandler()
{
	console() << "reset printer data------------------  " << endl;
	hidePrinterControls();
	printerBlock->setÑurrentPhotosPrinted(0);
	configSettings->setCurrentPhotosPrinted(0);
}

void MainConfig::hidePrinterControls()
{
	printerBlock->disconnectEventHandler(PrinterBlock::HIDE_EVENT);
	printerBlock->disconnectEventHandler(PrinterBlock::STAT_RESET_EVENT);
	printerBlock->connectEventHandler(&MainConfig::printerControlsHided, this, PrinterBlock::HIDED);

	logo->animateToDownState();
	printerBlock->closeControls();
}

void MainConfig::printerControlsHided()
{
	printerBlock->disconnectEventHandler(PrinterBlock::HIDED);
	activateListeners();
}

////////////////////////////////////////////////////////////////////////////
//
//				POPUP
//
////////////////////////////////////////////////////////////////////////////			

void MainConfig::openPopupNewActivity(EventGUIRef& event)
{
	unActivateListeners();
	addChild(newActPopup);
	newActPopup->connectEventHandler(&MainConfig::popupOpened, this, NewActivityPopup::OPENED);
	newActPopup->show();
}

void MainConfig::popupOpened()
{
	console() << "POPUP :: OPENED " << endl;
	newActPopup->disconnectEventHandler(NewActivityPopup::OPENED);
	newActPopup->connectEventHandler(&MainConfig::popupHideHandler, this, NewActivityPopup::HIDE_EVENT);
	newActPopup->connectEventHandler(&MainConfig::popupStartNewCompainHandler, this, NewActivityPopup::START_NEW_COMPAIN);
}

void MainConfig::popupHideHandler()
{
	closingPopup();
}

void MainConfig::popupStartNewCompainHandler()
{
	closingPopup();

	if (configSettings->getActionName() != newActPopup->getCompainName())
	{
		startNewCampain();
	}
	else
	{
		console() << "nothing changed------------------  " << endl;
	}
}

void MainConfig::startNewCampain()
{
	// TODO START NEW COMPAIN
	console() << "start new compain------------------  " << endl;
	title->setActivityName(newActPopup->getCompainName());
	configSettings->setActionName(newActPopup->getCompainName());

	statBlock->nullValues();
	configSettings->setPlayedCount(0);
	configSettings->setPrintedCount(0);
	configSettings->setPuplishedCount(0);
}

void MainConfig::closingPopup()
{
	newActPopup->disconnectEventHandler(NewActivityPopup::HIDE_EVENT);
	newActPopup->disconnectEventHandler(NewActivityPopup::START_NEW_COMPAIN);
	newActPopup->connectEventHandler(&MainConfig::popupClosed, this, NewActivityPopup::HIDED);
	newActPopup->hide();
}

void MainConfig::popupClosed()
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

void MainConfig::openingDesignLayout()
{
	console() << "========openingDesignLayout===========---------------------- " << endl;
	gamesBlockEnabled = false;
	unActivateListeners();
	
	designBlock->disconnectEventHandler(DesignBlock::OPEN_EVENT);
	//designBlock->disconnectEventHandler(DesignBlock::CLOSE_EVENT);
	designBlock->connectEventHandler(&MainConfig::openedDesignLayout, this, DesignBlock::CLOSE_EVENT);

	designBlock->connectEventHandler(&MainConfig::openedDesignLayout, this, DesignBlock::OPENED);
	gamesBlock->hide(EaseOutCubic(), 0.9f);
	designBlock->showDesigns(EaseOutCubic(), 0.9f);
}

void MainConfig::openedDesignLayout()
{	
	activateListeners();
	
	designBlock->setOpened(true);
	designBlock->disconnectEventHandler(DesignBlock::OPENED);
	//designBlock->activateListeners();

	designBlock->connectEventHandler(&MainConfig::callbacks, this);
	designBlock->connectEventHandler(&MainConfig::screenSaverStateChanged, this, DesignBlock::SCREEN_SAVER_STATE);
	designBlock->connectEventHandler(&MainConfig::screenSaverOpenFolder, this, DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
	designBlock->connectEventHandler(&MainConfig::hidingDesignLayout, this, DesignBlock::HIDE);
}

void MainConfig::callbacks(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if (!ev)
	{
		return;
	}

	if (typeid(*event.get()) == typeid(OpenSystemDirectoryEvent))
	{
		auto evt = static_pointer_cast<OpenSystemDirectoryEvent>(event);
		auto path = evt->getPath();
		console() << "OpenSystemDirectoryEvent url-------------" << path<< endl;
		fileTools().openSystemDirectory(path);	
	}
	else if (typeid(*event.get()) == typeid(ChangeDesignEvent))
	{
		auto evt = static_pointer_cast<ChangeDesignEvent>(event);
		auto id = evt->getItem().getID();
		configSettings->setActiveDesignID(id);
		console() << "changed kubik design------------------  " << endl;
	}
}

void MainConfig::screenSaverStateChanged()
{
	console() << "screen saver changed state------------------  " << designBlock->getScreenSaverValue() << endl;
	ssSettings->setActive(designBlock->getScreenSaverValue());
}

void MainConfig::screenSaverOpenFolder()
{
	console() << "screen saver open folder------------------  " << endl;
}

void MainConfig::openUserDesignFolder()
{
	console() << "open user design folder------------------  " << endl;
}

void MainConfig::hidingDesignLayout()
{
	console() << "hidingDesignLayout------------------  " << endl;
	unActivateListeners();

	designBlock->disconnectEventHandler();
	designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_STATE);
	designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
	designBlock->disconnectEventHandler(DesignBlock::HIDE);
	designBlock->connectEventHandler(&MainConfig::hideDesignLayot, this, DesignBlock::HIDED);
	designBlock->hideDesigns(EaseOutCubic(), 0.9f);
	gamesBlock->show(EaseOutCubic(), 0.9f);
}

void MainConfig::hideDesignLayot()
{
	gamesBlockEnabled = true;
	designBlock->setOpened(false);
	designBlock->disconnectEventHandler(DesignBlock::HIDED);
	activateListeners();
}

void MainConfig::closeDesignBlock()
{
	if (designBlock->getOpened())
	{
		gamesBlockEnabled = true;
		designBlock->setOpened(false);
		designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_STATE);
		designBlock->disconnectEventHandler(DesignBlock::SCREEN_SAVER_OPEN_FOLDER);
		designBlock->disconnectEventHandler(DesignBlock::HIDE);
		designBlock->disconnectEventHandler();
		designBlock->unActivateListeners();

		designBlock->hideDesigns(EaseOutCubic(), 0.2f);
		gamesBlock->show(EaseOutCubic(), 0.9f);
	}
}

////////////////////////////////////////////////////////////////////////////
//
//			GAMES BLOCK	
//
////////////////////////////////////////////////////////////////////////////

void MainConfig::gamesBlockEventsHandler(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if (!ev)
	{
		return;
	}

	if (typeid(*event.get()) == typeid(GameConfEvent))
	{
		auto eventref = static_pointer_cast<GameConfEvent>(event);
		console() << "go to config page------------------ " << eventref->getGameId() << endl;
		mouseUpSignal(event);
	}
	else if (typeid(*ev) == typeid(StatisticEvent))
	{
		auto eventref = static_pointer_cast<StatisticEvent>(event);
		auto url = gameSettings->getGameStatisticURL(eventref->getGameId());
		fileTools().openURL(url);
		console() << "show statistic game url------------------ " << url << endl;
		
	}
	else if (typeid(*ev) == typeid(GameCheckerEvent))
	{
		auto eventref = static_pointer_cast<GameCheckerEvent>(event);
		changeActiveGameState(eventref);	
	}
	else if (typeid(*ev) == typeid(GameShowUrlEvent))
	{
		auto eventref = static_pointer_cast<GameShowUrlEvent>(event);
		auto url = gameSettings->getGameDescribeURL(eventref->getGameId());
		fileTools().openURL(url);
		console() << "show game describe url------------------- " << url <<endl;
	}
}

void MainConfig::changeActiveGameState(GameCheckerEventRef eventref)
{
	if (gameSettings->getGameActiveCount() == 1)		
	{	
		if (eventref->getValue() == 0)
		{
			gamesBlock->freezeChecker(eventref->getGameId());
		}
		else
		{
			gameSettings->setGameActive(eventref->getGameId(), eventref->getValue());
			gamesBlock->unFreezeChecker();
		}	
	}
	else
	{
		gameSettings->setGameActive(eventref->getGameId(), eventref->getValue());
	}

	if (gameSettings->getGameActiveCount() == 1)
	{
		auto id = gameSettings->getActiveGameID();
		gamesBlock->freezeChecker(id);		
	}	
}

void MainConfig::setStartupData()
{	
	statBlock->setPlayedTimes(configSettings->getPlayedCount());
	statBlock->setPrintedPhotos(configSettings->getPrintedCount());
	statBlock->setSharedAndEmail(configSettings->getPublishedCount());
	printerBlock->setÑurrentPhotosPrinted(configSettings->getCurrentPhotosPrinted());
}

void MainConfig::draw()
{
	gl::color(ci::Color::hex(0x0d0917));
	gl::drawSolidRect(getWindowBounds());
	Sprite::draw();
}

void MainConfig::startAnimation(ci::EaseFn eFunc, float time)
{
	alpha = 0.0f;
	animatePosition = ci::Vec2f(1080.0f, 0.0f);

	Sprite::setAlpha(alpha);
	setPosition(animatePosition.value());

	timeline().apply(&alpha, 1.0f, time, eFunc)
		.updateFn(bind(&MainConfig::alphAnimationUpdate, this))
		.finishFn(bind(&MainConfig::startAnimationFinish, this));

	timeline().apply(&animatePosition, ci::Vec2f::zero(), time, eFunc)
		.updateFn(bind(&MainConfig::posAnimationUpdate, this));
}

void MainConfig::alphAnimationUpdate()
{
	Sprite::setAlpha(alpha);
	closeBlock->setAlpha(1);
}

void MainConfig::posAnimationUpdate()
{
	setPosition(animatePosition.value());
}

void MainConfig::startAnimationFinish()
{
	activateListeners();
}

void MainConfig::hideAnimate(GameId id, ci::EaseFn eFunc, float time)
{
	unActivateListeners();

	auto finPos = ci::Vec2f(-1080.0f + logo->getWidth(), 0.0f);
	timeline().apply(&animatePosition, finPos, time, eFunc)
		.updateFn(bind(&MainConfig::posAnimationUpdate, this));

	timeline().apply(&alpha, 0.0f, time, eFunc)
		.updateFn(bind(&MainConfig::alphAnimationUpdate, this))
		.finishFn(bind(&MainConfig::hideAnimationFinish, this));

	auto icon = gameSettings->getData().getPurchasedGameInfo(id).getIcons().activeIcon;
	logo->animateToMiniState(icon, eFunc, time);
	closeBlock->animateToMiniState(eFunc, time, finPos);
}

void MainConfig::hideAnimationFinish()
{
	closeBlock->connectEventHandler(&MainConfig::closeHandler, this);
}

void MainConfig::showAnimate(ci::EaseFn eFunc, float time)
{
	closeBlock->disconnectEventHandler();

	auto finPos = ci::Vec2f::zero();
	timeline().apply(&animatePosition, finPos, time, eFunc)
		.updateFn(bind(&MainConfig::posAnimationUpdate, this));

	timeline().apply(&alpha, 1.0f, time, eFunc)
		.updateFn(bind(&MainConfig::alphAnimationUpdate, this))
		.finishFn(bind(&MainConfig::showAnimationFinish, this));

	logo->animateToMaxState(eFunc, time);
	closeBlock->animateToMaxState(eFunc, time);
}

void MainConfig::showAnimationFinish()
{
	callback(SHOW_ANIM_COMPLETE);
	activateListeners();
}