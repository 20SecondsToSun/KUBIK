#include "states/choosingButtons/PhotoContainer.h"

using namespace kubik;
using namespace ci;
using namespace std;

PhotoContainer::PhotoContainer(int id, const ci::gl::Texture& tex1, const ci::gl::Texture& tex2, const ci::Vec2f& vec)
	:SimpleSpriteButton(Rectf(vec, vec + Vec2f(270, 350)), PhotoChoosedEventRef(new PhotoChoosedEvent(id))),
	galka(tex1),
	ramka(tex2),
	isSelected(false)
{

}

void PhotoContainer::setSelected(bool value)
{
	isSelected = value;
}

bool PhotoContainer::selected()
{
	return isSelected;
}

void PhotoContainer::drawLayout()
{
	gl::drawSolidRect(buttonArea);

	if (isSelected)
	{		
		gl::draw(ramka, Vec2f(-9, -10));	
		gl::draw(galka, Vec2f(29, 270));		
	}
}