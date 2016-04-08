#include "ScreenSaver.h"

using namespace std;
using namespace kubik;
using namespace kubik::config;

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
	connectEventHandler(&ScreenSaver::mouseUp, this);
	screenSaverResource->start();
}

void ScreenSaver::stop()
{	
	disconnectEventHandler();
	screenSaverResource->stop();
}

void ScreenSaver::draw()
{
	screenSaverResource->draw();
}

void ScreenSaver::mouseUp(EventGUIRef& event)
{
	closeLocationSignal();
}