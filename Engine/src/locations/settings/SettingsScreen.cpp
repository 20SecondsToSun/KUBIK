#include "SettingsScreen.h"

SettingsScreen::SettingsScreen():IScreen()
{	
	setTextures();
}

void SettingsScreen::setTextures()
{
	addToDictionary("helvetica90",  getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 90);
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
	font =  designTexures["helvetica90"]->font;
}

void SettingsScreen::draw()
{
	gl::color(Color(1,0,0));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	textTools().textFieldDraw("Õ¿—“–Œ… »", &font, Vec2f(100, 100), Color::white());
}