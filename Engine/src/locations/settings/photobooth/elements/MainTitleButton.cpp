#include "MainTitleButton.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

MainTitleButton::MainTitleButton(const Rectf& rect, int index):SimpleSpriteButton(rect)
{
	event = OpenPhotoBoothLayoutEventRef(new OpenPhotoBoothLayoutEvent(index));		
}	

void MainTitleButton::draw()
{

}