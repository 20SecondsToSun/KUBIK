#include "MenuScreen.h"
#include "Resources.h"
#include "SettingsFactory.h"

using namespace kubik;
using namespace kubik::menu;
using namespace kubik::config;
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

const float MenuScreen::goToScreenSaverTime = 6.0f;

MenuScreen::MenuScreen(ISettingsRef config):IScreen(ScreenId::MENU)
{
	init(config);
}

MenuScreen::~MenuScreen()
{
	clearGamesButtonVector();	
}

void MenuScreen::init(ISettingsRef value)
{	
	settings = static_pointer_cast<MenuSettings>(value);
	bckgnd   = settings->getTexture("background");
	createMenuBtns(settings->getEnabledGamesData());
}

void MenuScreen::resetMenuBtnGames()
{	
	clearGamesButtonVector();
	createMenuBtns(settings->getEnabledGamesData());
}

void MenuScreen::createMenuBtns(const std::vector<GameData>& games)
{
	if (games.size() <= 1)
		return;

	AdditionalGameData additionalGD = settings->getMenuScreenAdditionalDesignElements();

	for (auto it : games)
	{			
		GameButtonRef button = GameButtonRef(new GameButton(it,
			additionalGD.getBackground(),
			additionalGD.getBackgroundPosition(),
			additionalGD.getTitleByID(it.getID()),
			additionalGD.getTitlePosition()));
		addChild(button);
		gamesBtns.push_back(button);		
	}
}

void MenuScreen::clearGamesButtonVector()
{
	removeAllChildren();

	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();	

	gamesBtns.clear();
}

void MenuScreen::start()
{	
	if (getScreenShot())
	{
		state = SHOW_ANIM;
		timeline().apply(&animX, 0.0f, 1080.0f, 0.9f, EaseOutCubic()).finishFn(bind(&MenuScreen::showAnimationComplete, this));
		timeline().apply(&animX1, -500.0f, -0.0f, 0.9f, EaseOutCubic());
		timeline().apply(&alpha, 0.0f, 1.0f, 0.9f, EaseOutCubic());
	}		
	else
	{
		state = INIT_ANIM;
		timeline().apply(&alpha, 0.0f, 1.0f, 0.9f, EaseOutCubic()).finishFn(bind(&MenuScreen::showAnimationComplete, this));
	}

	if (settingsFactory().getScreenSaver()->isShow())
		delaycall(bind(&MenuScreen::gotoScreeenSaverTimeOut, this), goToScreenSaverTime);
}

void MenuScreen::showAnimationComplete()
{
	state = DRAW;

	for (auto btn : gamesBtns)
		btn->connectEventHandler(&MenuScreen::startGameHandler, this);

	//callback(ENABLE_GAME_CLOSE);
}

void MenuScreen::gotoScreeenSaverTimeOut()
{
	startVideoSignal();
}

void MenuScreen::stop()
{
	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();	

	clearDelaycall();
}

void MenuScreen::startGameHandler(EventGUIRef& evt )
{	
	EventGUI *ev = evt.get();
	if (!ev) return;

	if (typeid(*ev) == typeid(GameChoosedEvent))
	{		
		GameChoosedEventRef event = static_pointer_cast<GameChoosedEvent>(evt);	
		auto id = event->getGameID();
		startGameSignal(id);
	}
}

void MenuScreen::draw()
{
	gl::draw(bckgnd, getWindowBounds());

	switch (state)
	{
	case MenuScreen::SHOW_ANIM:
		drawShowAnim();
		break;

	case MenuScreen::INIT_ANIM:
		drawInitAnim();
		break;

	case MenuScreen::DRAW:
		Sprite::draw();
		break;
	}	
}

void MenuScreen::drawShowAnim()
{
	gl::pushMatrices();
	gl::translate(animX, 0.0f);	
	drawScreenShot();
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate(animX1, 0.0f);
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
	Sprite::draw();
	gl::color(Color::white());
	gl::popMatrices();	
}

void MenuScreen::drawInitAnim()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
	Sprite::draw();	
}