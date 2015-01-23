#include "MenuScreen.h"
#include "Resources.h"

using namespace kubik;

MenuScreen::MenuScreen(ISettingsRef config):IScreen(ScreenId::MENU)
{
	init(config);
}

MenuScreen::~MenuScreen()
{
	clearButtonVector();
	removeMouseUpListener();
	console()<<"~~~~~~~~~~~~~~~~Menu screen destructor~~~~~~~~~~~~~~~~"<<endl;
}

void MenuScreen::init(ISettingsRef  _settings)
{	
	settings	   =  static_pointer_cast<MenuSettings>(_settings);
	font		   =  settings->getFont("helvetica30");
	bckgnd         =  settings->getTexture("background");

	createMenuBtns(settings->getGames());
}

void MenuScreen::resetMenuBtnGames()
{
	clearButtonVector();
	createMenuBtns(settings->getGames());
}

void MenuScreen::createMenuBtns(vector<GamesInfo> games)
{
	string settingsName	   = "Настройки";
	string screenSaverName = "Заставка";
	
	int i = 0;

	for(auto it : games)
	{
		if(it.isOn && it.isPurchased)
		{
			MenuButtonRef button = MenuButtonRef(new MenuButton(getMenuBtuttonArea(i++), it.name, font, it.id));		
			button->addMouseUpListener(&MenuScreen::gameMouseUpListener, this);
			addChild(button);
		}
	}	

	Rectf setButtonArea = Rectf(50.0f, 50.0f, 350.0f, 150.0f);
	settingsButton = TextButtonRef(new TextButton(setButtonArea, settingsName, font));	
	settingsButton->addMouseUpListener(&MenuScreen::settingsMouseUpListener, this);
	addChild(settingsButton);

	console()<<"draw::::::::::::::  "<<settingsButton->getLocalPosition()<<"   "<<settingsButton->getParentPosition()<<endl;

	Rectf videoButtonArea = Rectf(400.0f, 50.0f, 700.0f, 150.0f);
	videoButton = TextButtonRef(new TextButton(videoButtonArea, screenSaverName, font));	
	videoButton->addMouseUpListener(&MenuScreen::videoMouseUpListener, this);
	addChild(videoButton);

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

void MenuScreen::clearButtonVector()
{
	for(auto it : displayList)	
		it->removeMouseUpListener();
}

void MenuScreen::start()
{
	addMouseUpListener(&MenuScreen::mouseUpFunction, this);
}

void MenuScreen::stop()
{	
	removeMouseUpListener();	
}

void MenuScreen::gameMouseUpListener(EventGUIRef& evt )
{	
	GameChoosedEventRef event = static_pointer_cast<GameChoosedEvent>(evt);	
	startGameSignal(event->getGameID());
}

void MenuScreen::settingsMouseUpListener(EventGUIRef& evt )
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
	CompositeDispatcher::draw();	
}