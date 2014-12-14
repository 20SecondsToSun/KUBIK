#include "MenuScreen.h"
#include "Resources.h"

using namespace kubik;

MenuScreen::MenuScreen(shared_ptr<ISettings>  settings)
{
	init(settings);
}

MenuScreen::~MenuScreen()
{
	clearButtonVector();
	mouseListener.disconnect();
	console()<<"~~~~~~~~~~~~~~~~Menu screen destructor~~~~~~~~~~~~~~~~"<<endl;
}

void MenuScreen::init(shared_ptr<ISettings>  _settings)
{	
	settings	   =  static_pointer_cast<MenuSettings>(_settings);	
	font		   =  settings->getFonts()["helvetica30"]->get();
	bckgnd         =  settings->getTextures()["background"]->get();

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

		int btnId    = it.id;
		float x      = 300.0f *(1 + i++);
		float y      = 400.0f;
		float width  = 200.0f;
		float height = 200.0f;

		Rectf buttonArea = Rectf(x, y, x + width, y + height);
		shared_ptr<MenuButton> button = shared_ptr<MenuButton>(new MenuButton(btnId, buttonArea, it.name, font));		
		connect_once(button->mouseUpSignal, bind(&MenuScreen::mouseUpListener, this, std::placeholders::_1));
		menuBtns.push_back(button);
	}	

	settingsButton = shared_ptr<ButtonText>(new ButtonText(Rectf(50.0f, 50.0f, 350.0f, 150.0f), settingsName, font));	
	connect_once(settingsButton->mouseUpSignal, bind(&MenuScreen::settingsMouseUpListener, this, std::placeholders::_1));

	videoButton = shared_ptr<ButtonText>(new ButtonText(Rectf(400.0f, 50.0f, 700.0f, 150.0f), screenSaverName, font));	
	connect_once(videoButton->mouseUpSignal, bind(&MenuScreen::videoMouseUpListener, this, std::placeholders::_1));
}

void MenuScreen::clearButtonVector()
{
	for(auto it : menuBtns)	
		it->mouseUpSignal.disconnect_all_slots();

	menuBtns.clear();

	videoButton->mouseUpSignal.disconnect_all_slots();
	settingsButton->mouseUpSignal.disconnect_all_slots();
}

void MenuScreen::mouseUpListener(MenuButton& button )
{	
	startGameSignal(button.getGameId());
}

void MenuScreen::settingsMouseUpListener(ButtonText& button )
{
	startSettingsSignal();
}

void MenuScreen::videoMouseUpListener(ButtonText& button )
{
	startVideoSignal();
}

void MenuScreen::mouseUp( MouseEvent &event)
{	
	for(auto it : menuBtns)
		it->mouseUpHandler(event.getPos());

	settingsButton->mouseUpHandler(event.getPos());
	videoButton->mouseUpHandler(event.getPos());
}

void MenuScreen::draw()
{
	gl::draw(bckgnd, getWindowBounds());

	for(auto it : menuBtns)
		it->draw();

	settingsButton->draw();
	videoButton->draw();
}

void MenuScreen::removeMouseUpListener()
{
	mouseListener.disconnect();
}

void MenuScreen::addMouseUpListener()
{
	mouseListener = getWindow()->getSignalMouseUp().connect(std::bind( &MenuScreen::mouseUp, this, std::placeholders::_1));
}