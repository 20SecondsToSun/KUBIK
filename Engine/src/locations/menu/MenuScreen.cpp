#include "MenuScreen.h"
#include "Resources.h"

using namespace kubik;
using namespace kubik::menu;
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

MenuScreen::MenuScreen(ISettingsRef config):IScreen(ScreenId::MENU)
{
	init(config);
}

MenuScreen::~MenuScreen()
{
	clearGamesButtonVector();	
	console()<<"~~~~~~~~~~~~~~~~Menu screen destructor~~~~~~~~~~~~~~~~"<<endl;
}

void MenuScreen::init(ISettingsRef  _settings)
{	
	settings	   =  static_pointer_cast<MenuSettings>(_settings);
	font		   =  settings->getFont("helvetica30");
	bckgnd         =  settings->getTexture("background");

	createMenuBtns(settings->getGames());
	createControlsButtons();
}

void MenuScreen::resetMenuBtnGames()
{
	clearGamesButtonVector();
	createMenuBtns(settings->getGames());
}

void MenuScreen::createMenuBtns(const std::vector<GamesInfo>& games)
{		
	int i = 0;
	for(auto it : games)
	{
		if(it.isOn && it.isPurchased)
		{
			SimpleSpriteButtonRef button = SimpleSpriteButtonRef(new SimpleSpriteButton(getMenuBtuttonArea(i++)));//, it.name, font, it.id));		
			//button->addMouseUpListener(&MenuScreen::gameMouseUpListener, this);
			addChild(button);
			gamesBtns.push_back(button);
		}
	}
}

void MenuScreen::createControlsButtons()
{
	Rectf setButtonArea = Rectf(50.0f, 50.0f, 350.0f, 150.0f);
	settingsButton = SimpleSpriteButtonRef(new SimpleSpriteButton(setButtonArea));//, settingsName, font));	
	//settingsButton->addMouseUpListener(&MenuScreen::settingsMouseUpListener, this);
	addChild(settingsButton);


	/*Rectf videoButtonArea = Rectf(400.0f, 50.0f, 700.0f, 150.0f);
	videoButton = TextButtonRef(new TextButton(videoButtonArea, screenSaverName, font));	
	videoButton->addMouseUpListener(&MenuScreen::videoMouseUpListener, this);
	addChild(videoButton);*/

	//videoButton->setPosition(Vec2f(800,800));
	//setPosition(Vec2f(-400,0));
}

Rectf MenuScreen::getMenuBtuttonArea(int i)
{
	float x      = 300.0f * (1 + i);
	float y      = 400.0f;
	float width  = 200.0f;
	float height = 200.0f;
	return  Rectf(x, y, x + width, y + height);
}

void MenuScreen::clearGamesButtonVector()
{
	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();	
	gamesBtns.clear();
}

void MenuScreen::start()
{	
	settingsButton->connectEventHandler(&MenuScreen::startSettingsHandler, this);

	for(auto btn : gamesBtns)	
		btn->connectEventHandler(&MenuScreen::startGameHandler, this);
}

void MenuScreen::stop()
{
	settingsButton->disconnectEventHandler();

	for(auto btn : gamesBtns)	
		btn->disconnectEventHandler();		
}

void MenuScreen::startGameHandler(EventGUIRef& evt )
{	
	console()<<"startGameHandler!!!!!!!!!!!!"<<endl;
	//GameChoosedEventRef event = static_pointer_cast<GameChoosedEvent>(evt);	
	startGameSignal(GameId::PHOTOBOOTH);//event->getGameID());
}

void MenuScreen::startSettingsHandler(EventGUIRef& evt )
{
	startSettingsSignal();
}

void MenuScreen::videoMouseUpListener(EventGUIRef& evt )
{	
	startVideoSignal();
}

void MenuScreen::draw()
{
	gl::draw(bckgnd, getWindowBounds());
	Sprite::draw();	
}