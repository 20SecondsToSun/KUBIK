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

void SettingsScreen::init()
{

}

void SettingsScreen::draw()
{
}