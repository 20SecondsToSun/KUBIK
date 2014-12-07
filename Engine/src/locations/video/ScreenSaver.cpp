#include "ScreenSaver.h"
using namespace kubik;

ScreenSaver::ScreenSaver(ScreenSaverSettings* settings)
{	
	this->settings = settings;

	mode = this->settings->getScreenSaverMode();
	path_ss = this->settings->getScreenSaverPath();
	setTextures();
}

void ScreenSaver::setTextures()
{
	if(mode == IMAGE_SS)
	{
		addToDictionary("image", path_ss, resourceType::IMAGE, loadingType::FULL_PATH);		
	}		
	else if(mode == VIDEO_SS)
	{
		addToDictionary("video", path_ss, resourceType::VIDEO, loadingType::FULL_PATH);
	}	
}

void ScreenSaver::init()
{
	if(mode == VIDEO_SS)
	{
		screenSaverResource =  new VideoScreenSaver(designTexures["video"]->movie);	
	}
	else if(mode == IMAGE_SS)
	{
		screenSaverResource =  new ImageScreenSaver(designTexures["image"]->tex);	
	}
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