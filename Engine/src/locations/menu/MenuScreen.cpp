#include "MenuScreen.h"

MenuScreen::MenuScreen(vector<int> gameIDs)
{	
	setTextures();
	createMenuBtns(gameIDs);	
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
	clearButtonVector();
	int i = 0;
	for(auto it = gameIDs.begin(); it != gameIDs.end(); ++it)
	{
		int btnId    = *it;
		float x      = 300.0f *(1 + i++);
		float y      = 200.0f;
		float width  = 200.0f;
		float height = 200.0f;

		Rectf buttonArea = Rectf(x, y , x + width, y + height);

		MenuButton *button = new MenuButton(btnId, buttonArea);		
		button->mouseUpSignal.connect(bind(&MenuScreen::mouseUpListener, this, std::placeholders::_1));
		menuBtns.push_back(button);
	}
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

void MenuScreen::mouseUp( MouseEvent &event)
{	
	for(auto it = menuBtns.begin(); it != menuBtns.end(); ++it)
		(*it)->mouseUpHandler(event.getPos());
}

void MenuScreen::draw()
{
	for(auto it = menuBtns.begin(); it != menuBtns.end(); ++it)
		(*it)->draw();
}

void MenuScreen::removeMouseUpListener()
{
	mouseListener.disconnect();
}

void MenuScreen::addMouseUpListener()
{
	mouseListener = getWindow()->getSignalMouseUp().connect( std::bind( &MenuScreen::mouseUp, this, std::placeholders::_1) );
}
