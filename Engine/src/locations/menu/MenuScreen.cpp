#include "MenuScreen.h"
#include "Resources.h"

using namespace kubik;

void MenuScreen::reload()
{
	clean();
}

void MenuScreen::clean()
{
	clearButtonVector();
}

void MenuScreen::init(MenuSettings* settings)
{	
	this->settings = settings;
	font		   =  settings->getTextures()["helvetica30"]->font;
	bckgnd         =  settings->getTextures()["background"]->tex;

	createMenuBtns(settings->getGameIDs());
}

void MenuScreen::createMenuBtns(vector<int> gameIDs)
{
	string gameNames[3] = {"Funces", "Photobooth", "Kotopoza"};
	string settingsName = "Настройки";
	string screenSaverName = "Заставка";	

	int i = 0;

	for(auto it : gameIDs)
	{
		int btnId    = it;
		float x      = 300.0f *(1 + i);
		float y      = 400.0f;
		float width  = 200.0f;
		float height = 200.0f;

		Rectf buttonArea = Rectf(x, y, x + width, y + height);
		MenuButton *button = new MenuButton(btnId, buttonArea, gameNames[i], font);		
		connect_once(button->mouseUpSignal, bind(&MenuScreen::mouseUpListener, this, std::placeholders::_1));
		menuBtns.push_back(button);
		i++;
	}	

	settingsButton = new ButtonText(Rectf(50.0f, 50.0f, 350.0f, 150.0f), settingsName, font);	
	connect_once(settingsButton->mouseUpSignal, bind(&MenuScreen::settingsMouseUpListener, this, std::placeholders::_1));

	videoButton = new ButtonText(Rectf(400.0f, 50.0f, 700.0f, 150.0f), screenSaverName, font);	
	connect_once(videoButton->mouseUpSignal, bind(&MenuScreen::videoMouseUpListener, this, std::placeholders::_1));
}

void MenuScreen::clearButtonVector()
{
	for(auto it : menuBtns)
	{
		it->mouseUpSignal.disconnect_all_slots();
		delete it;
	}
	menuBtns.clear();

	videoButton->mouseUpSignal.disconnect_all_slots();
	settingsButton->mouseUpSignal.disconnect_all_slots();	

	delete settingsButton;
	delete videoButton;
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
	gl::draw(bckgnd);

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
	mouseListener = getWindow()->getSignalMouseUp().connect( std::bind( &MenuScreen::mouseUp, this, std::placeholders::_1) );
}