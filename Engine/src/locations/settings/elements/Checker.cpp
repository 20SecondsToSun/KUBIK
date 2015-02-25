#include "Checker.h"

using namespace kubik;
using namespace kubik::config;

Checker::Checker(const ci::Rectf& rect, const IconPair& icons, ci::Color activeColor, ci::Color unActiveColor)
	:SimpleSpriteButton(rect),
	activeBordercolor(ci::Color::hex(0xbc6d10)),
	unActiveBordercolor(ci::Color::hex(0xa05b09)),
	icons(icons),
	icon(icons.unActiveIcon),
	isActive(false),				
	activeColor(activeColor),
	unActiveColor(unActiveColor),
	color(unActiveColor),
	radius(55.0f),
	startX(0.0f),
	finishX(77.0f),
	sdvigX(0.0f),
	iconColor(ci::Color::white())
{
	event = CheckerEventRef(new CheckerEvent(isActive));
}	

void Checker::mouseUp(ci::app::MouseEvent &_event)
{
	if(inButtonField(_event.getPos()))
	{
		swapActive();
		CheckerEventRef eventref = static_pointer_cast<CheckerEvent>(event);
		eventref->setActive(isActive);	
		Sprite::mouseUp(_event);
	}
}

void Checker::drawLayout()
{
	gl::color(color);
	gl::drawSolidRoundedRect(buttonArea, radius, 200.0f);
	gl::color(Color::white());
	gl::color(borderColor);
	gl::drawStrokedRoundedRect(buttonArea, radius, 200.0f);
	gl::color(Color::white());
	gl::pushMatrices();		
	gl::translate(sdvigX + buttonArea.x1, buttonArea.y1 - 3.0f);
	gl::color(iconColor);
	gl::draw(icon);
	gl::popMatrices();							
}

void Checker::setAlpha(float  alpha)
{
	color	  = Utils::colorAlpha(color, alpha);
	iconColor = Utils::colorAlpha(iconColor, alpha);
}

void Checker::setActive(bool isActive)
{
	this->isActive = isActive;

	if(isActive)
	{
		color  = activeColor;
		borderColor = activeBordercolor;
		icon   = icons.activeIcon;
		sdvigX = finishX;
	}
	else
	{
		color  = unActiveColor;
		borderColor = unActiveBordercolor;
		icon   = icons.unActiveIcon;
		sdvigX = startX;
	}
}		

bool Checker::getValue() const
{
	return isActive;
}

void Checker::setRadius(float radius)
{
	this->radius = radius;
}

void Checker::setStartX(float x)
{
	this->startX = x;
}

void Checker::setFinishX(float x)
{
	this->finishX = x;
}

void Checker::swapActive()
{
	setActive(!isActive);
}

void Checker::setBorderColorActive(const ci::ColorA& color)
{
	activeBordercolor = color;
	setActive(isActive);
}

void Checker::setBorderColorUnActive(const ci::ColorA& color)
{
	unActiveBordercolor = color;
	setActive(isActive);
}	