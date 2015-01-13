#include "ScreenSaver.h"

using namespace kubik;

ScreenSaver::ScreenSaver(ISettingsRef config):IScreen(ScreenId::SCREENSAVER)
{
	init(config);
}

void ScreenSaver::init(ISettingsRef config)
{
	settings =  static_pointer_cast<ScreenSaverSettings>(config);
	screenSaverResource = settings->getResource();	
}

void ScreenSaver::start()
{
	addMouseUpListener();
	screenSaverResource->start();
}

void ScreenSaver::stop()
{	
	removeMouseUpListener();
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