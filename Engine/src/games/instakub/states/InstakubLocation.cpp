#include "InstakubLocation.h"

using namespace kubik;
using namespace ci;
using namespace kubik::games::instakub;

InstagramViewRef InstakubLocation::instagramView;
gl::Texture InstakubLocation::bg;

InstakubLocation::InstakubLocation(InstakubSettingsRef settings)
	:settings(settings)
{
	if(!instagramView)
		instagramView = InstagramViewRef(new InstagramView(settings->getTexture("closeInstaPopup"),
		settings->getTexture("printInstaPopup"),
		settings->getCurrentTemplate()));
}

void InstakubLocation::start()
{
	instagramView->connectEventHandler(&InstakubLocation::enableControls, this, ENABLE_CONTROLS);
	instagramView->connectEventHandler(&InstakubLocation::disableControls, this, DISABLE_CONTROLS);
	instagramView->connectEventHandler(&InstakubLocation::showControls, this, SHOW_CONTROLS);
	instagramView->connectEventHandler(&InstakubLocation::hideControls, this, HIDE_CONTROLS);
	instagramView->start();
}

void InstakubLocation::stop()
{
	instagramView->disconnectEventHandler(ENABLE_CONTROLS);
	instagramView->disconnectEventHandler(DISABLE_CONTROLS);
	instagramView->disconnectEventHandler(SHOW_CONTROLS);
	instagramView->disconnectEventHandler(HIDE_CONTROLS);
	instagramView->stop();
}

void InstakubLocation::enableControls()
{
	callback(ENABLE_CONTROLS);
}

void InstakubLocation::disableControls()
{
	callback(DISABLE_CONTROLS);
}

void InstakubLocation::showControls()
{
	callback(SHOW_CONTROLS);
}

void InstakubLocation::hideControls()
{
	callback(HIDE_CONTROLS);
}

void InstakubLocation::draw()
{
	fillBg();
	drawTitle();
};

void InstakubLocation::reset()
{
	bg = settings->getTexture("bg");	
}

void InstakubLocation::fillBg()
{
	gl::draw(bg);
};

void InstakubLocation::drawTitle()
{

};