#include "SettingsScreen.h"

SettingsScreen::SettingsScreen()
{	
	setTextures();
	//mouseListener = getWindow()->getSignalMouseUp().connect( std::bind( &MenuScreen::mouseUp, this, std::placeholders::_1) );
}

void SettingsScreen::setTextures()
{
	addToDictionary("background" , "settingsDesign\\bg1.png");
}

void SettingsScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&SettingsScreen::mouseUp, this);
}

void SettingsScreen::mouseUp(MouseEvent &event)
{
	closeSettingsSignal();
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
}