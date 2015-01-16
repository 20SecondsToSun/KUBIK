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
		if(!it.isOn || !it.isPurchased)
			continue;

		float x      = 300.0f *(1 + i++);
		float y      = 400.0f;
		float width  = 200.0f;
		float height = 200.0f;

		Rectf buttonArea = Rectf(x, y, x + width, y + height);
		MenuButtonRef button = MenuButtonRef(new MenuButton(it.id, buttonArea, it.name, font));		
		button->addMouseUpListener(&MenuScreen::gameMouseUpListener, this);
		displayList.push_back(button);
	}	

	Rectf setButtonArea = Rectf(50.0f, 50.0f, 350.0f, 150.0f);
	settingsButton = ButtonTextRef(new ButtonText(setButtonArea, settingsName, font));	
	settingsButton->addMouseUpListener(&MenuScreen::settingsMouseUpListener, this);
	displayList.push_back(settingsButton);

	Rectf videoButtonArea = Rectf(400.0f, 50.0f, 700.0f, 150.0f);
	videoButton = ButtonTextRef(new ButtonText(videoButtonArea, screenSaverName, font));	
	videoButton->addMouseUpListener(&MenuScreen::videoMouseUpListener, this);
	displayList.push_back(videoButton);
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

void MenuScreen::gameMouseUpListener(EventRef& button )
{	
	//startGameSignal(button.getGameId());
}

void MenuScreen::settingsMouseUpListener(EventRef& button )
{
	if(isMouseUpSet)
		startSettingsSignal();
}

void MenuScreen::videoMouseUpListener(EventRef& button )
{
	if(isMouseUpSet)
		startVideoSignal();
}

void MenuScreen::draw()
{
	gl::draw(bckgnd, getWindowBounds());

	for(auto it : displayList)
		it->draw();
}