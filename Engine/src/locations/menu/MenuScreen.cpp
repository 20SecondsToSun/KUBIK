#include "MenuScreen.h"
#include "Resources.h"

MenuScreen::MenuScreen(vector<int> gameIDs)
{	
	font =  Font(loadFile(getAssetPath("fonts/Helvetica Neue.ttf")), 30);	

	setTextures();
	createMenuBtns(gameIDs);

	//font =  Font( ci::app::App::loadResource( RES_CUSTOM_FONT ), 72 );
	//font =  Font( ci::app::App::loadResource(  3, "FONT" ), 72 );	
}

void MenuScreen::setTextures()
{
	addToDictionary("background" , "menuDesign\\bg.jpg");
	addToDictionary("background1" , "menuDesign\\title.jpg");
}

void MenuScreen::init()
{

}

void MenuScreen::createMenuBtns(vector<int> gameIDs)
{
	string gameNames[3] = {"Funces", "Photobooth", "Kotopoza"};
	string settingsName = "Настройки";
	string screenSaverName = "Заставка";
	
	clearButtonVector();

	int i = 0;

	for(auto it = gameIDs.begin(); it != gameIDs.end(); ++it)
	{
		int btnId    = *it;
		float x      = 300.0f *(1 + i);
		float y      = 400.0f;
		float width  = 200.0f;
		float height = 200.0f;

		Rectf buttonArea = Rectf(x, y, x + width, y + height);

		MenuButton *button = new MenuButton(btnId, buttonArea, gameNames[i], font);		
		button->mouseUpSignal.connect(bind(&MenuScreen::mouseUpListener, this, std::placeholders::_1));
		menuBtns.push_back(button);
		i++;
	}

	settingsButton = new ButtonText(Rectf(50.0f, 50.0f, 350.0f, 150.0f), settingsName, font);	
	settingsButton->mouseUpSignal.connect(bind(&MenuScreen::settingsMouseUpListener, this, std::placeholders::_1));

	videoButton = new ButtonText(Rectf(400.0f, 50.0f, 700.0f, 150.0f), screenSaverName, font);	
	videoButton->mouseUpSignal.connect(bind(&MenuScreen::videoMouseUpListener, this, std::placeholders::_1));
}

void MenuScreen::clearButtonVector()
{
	for(auto it = menuBtns.begin(); it != menuBtns.end(); ++it)
		delete *it;
	menuBtns.clear();
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
	for(auto it = menuBtns.begin(); it != menuBtns.end(); ++it)
		(*it)->mouseUpHandler(event.getPos());

	settingsButton->mouseUpHandler(event.getPos());
	videoButton->mouseUpHandler(event.getPos());
}

void MenuScreen::draw()
{
	for(auto it = menuBtns.begin(); it != menuBtns.end(); ++it)
		(*it)->draw();

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