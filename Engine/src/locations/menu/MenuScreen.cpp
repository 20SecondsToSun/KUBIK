#include "MenuScreen.h"
#include "Resources.h"

using namespace kubik;
using namespace kubik::menu;
using namespace kubik::config;
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

MenuScreen::MenuScreen(ISettingsRef config)
	:IScreen(ScreenId::MENU)
{
	init(config);
}

MenuScreen::~MenuScreen()
{
	clearGamesButtonVector();	
	console()<<"~~~~~~~~~~~~~~~~Menu screen destructor~~~~~~~~~~~~~~~~"<<endl;
}

void MenuScreen::init(ISettingsRef _settings)
{	
	settings	   =  static_pointer_cast<MenuSettings>(_settings);
	bckgnd         =  settings->getTexture("background");

	createMenuBtns(settings->getEnabledGamesData());
}

void MenuScreen::resetMenuBtnGames()
{
	auto games = settings->getEnabledGamesData();
	if (games.size() <= 1) return;

	clearGamesButtonVector();
	createMenuBtns(settings->getEnabledGamesData());
}

void MenuScreen::createMenuBtns(const std::vector<GameData>& games)
{	
	AdditionalGameData additionalGD = settings->getMenuScreenAdditionalDesignElements();

	for(auto it : games)
	{	
		GameButtonRef button = GameButtonRef(new GameButton(it,
			additionalGD.getBackground(),
			additionalGD.getBackgroundPosition(),
			additionalGD.getTitleByID(it.getID()),
			additionalGD.getTitlePosition()
			));
		addChild(button);
		gamesBtns.push_back(button);		
	}
}

//Rectf MenuScreen::getMenuBtuttonArea(int i)
//{
//	float x      = 300.0f * (1 + i);
//	float y      = 400.0f;
//	float width  = 200.0f;
//	float height = 200.0f;
//	return Rectf(x, y, x + width, y + height);
//}

void MenuScreen::clearGamesButtonVector()
{
	removeAllChildren();

	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();	

	gamesBtns.clear();
}

void MenuScreen::start()
{	
	screenshot = getScreenShot();
	if (screenshot)
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
}

void MenuScreen::showAnimationComplete()
{
	state = DRAW;

	for (auto btn : gamesBtns)
		btn->connectEventHandler(&MenuScreen::startGameHandler, this);
}

void MenuScreen::stop()
{
	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();		
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

void MenuScreen::videoMouseUpListener(EventGUIRef& evt )
{	
	startVideoSignal();
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
	gl::draw(screenshot);	
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