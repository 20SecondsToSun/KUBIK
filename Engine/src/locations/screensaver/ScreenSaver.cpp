#include "ScreenSaver.h"

using namespace kubik;

void ScreenSaver::init(ScreenSaverSettings* settings)
{
	this->settings = settings;
	screenSaverResource = settings->getResource();	
}

void ScreenSaver::start()
{
	screenSaverResource->start();
}

void ScreenSaver::stop()
{	
	screenSaverResource->stop();
}

void ScreenSaver::draw()
{
	screenSaverResource->draw();
}

void ScreenSaver::mouseUp(MouseEvent &event)
{
	closeLocationSignal();
}

void ScreenSaver::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&ScreenSaver::mouseUp, this);
}

void ScreenSaver::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}