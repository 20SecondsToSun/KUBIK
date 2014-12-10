#include "ScreenSaver.h"

using namespace kubik;

ScreenSaver::ScreenSaver(shared_ptr<ISettings> config)
{
	init(config);
}

void ScreenSaver::init(shared_ptr<ISettings> config)
{
	settings =  static_pointer_cast<ScreenSaverSettings>(config);
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