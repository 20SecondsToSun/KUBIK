#include "ApplicationView.h"

using namespace kubik;

void ApplicationView::startLocation(IScreen* screen)
{	
	location = screen;
}

void ApplicationView::draw()
{
	location->draw();
}