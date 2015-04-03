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