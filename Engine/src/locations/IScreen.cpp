#include "IScreen.h"

using namespace ci::signals;
using namespace std;
using namespace kubik;

IScreen::IScreen(const ScreenId& id) : id(id)
{

}

ScreenId IScreen::getType() const
{
	return id;
}

void IScreen::setType(const ScreenId& id)
{
	this->id = id;
}

void IScreen::draw()
{
};

void IScreen::start()
{

};

void IScreen::stop()
{

};

void IScreen::setScrenshot()
{	
	kubik::setScreenShot(Utils::drawGraphicsToFBO(ci::app::getWindowSize(), [&](){ draw(); }));
}