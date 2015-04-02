#include "InstakubLocation.h"

using namespace kubik;
using namespace kubik::games::instakub;

InstagramViewRef InstakubLocation::instagramView;

InstakubLocation::InstakubLocation(InstakubSettingsRef settings)
	:settings(settings),
	bg(settings->getTexture("bg"))
{
	if(!instagramView)
		instagramView = InstagramViewRef(new InstagramView());	
}

void InstakubLocation::draw()
{
	fillBg();
	drawTitle();
};

void InstakubLocation::fillBg()
{
	gl::draw(bg);
};

void InstakubLocation::drawTitle()
{

};