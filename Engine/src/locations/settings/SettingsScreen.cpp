#include "SettingsScreen.h"

SettingsScreen::SettingsScreen()
{	
	font =  Font(loadFile(getAssetPath("fonts/Helvetica Neue.ttf")), 70);
	setTextures();
	//mouseListener = getWindow()->getSignalMouseUp().connect( std::bind( &MenuScreen::mouseUp, this, std::placeholders::_1) );
}

void SettingsScreen::setTextures()
{
	//addToDictionary("background" , "settingsDesign\\bg1.png");
}

void SettingsScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&SettingsScreen::mouseUp, this);
}

void SettingsScreen::mouseUp(MouseEvent &event)
{
	closeLocationSignal();
}

void SettingsScreen::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void SettingsScreen::init()
{

}

void SettingsScreen::draw()
{
	gl::color(Color(1,0,0));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	textTools().textFieldDraw("Õ¿—“–Œ… »", &font, Vec2f(100, 100), Color::white());
}