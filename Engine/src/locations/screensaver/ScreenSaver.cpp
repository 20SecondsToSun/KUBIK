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
	addMouseUpListener(&ScreenSaver::mouseUpFunction, this);
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