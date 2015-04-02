#include "instakub/states/InstagramView.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

InstagramView::InstagramView() 
	:yPosition(0.0f)
{
	console() << "================================= CREATE ISTAGRAM VIEW =====================================" << endl;
}

void InstagramView::setYPosition(float y)
{
	yPosition = y;
}

void InstagramView::draw()
{
	gl::pushMatrices();
	gl::translate(0, yPosition);
	gl::color(Color::white());
	gl::drawSolidRect(Rectf(0, 0, getWindowWidth(), 800));
	gl::popMatrices();
}