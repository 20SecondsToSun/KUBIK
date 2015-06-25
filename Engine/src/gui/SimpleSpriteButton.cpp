#include "SimpleSpriteButton.h"


using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

SimpleSpriteButton::SimpleSpriteButton(const ci::Rectf& rect, EventGUIRef e)
	:Sprite(), color(ci::Color::white()), bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
{
	buttonArea = Rectf(0.0f, 0.0f, rect.getWidth(), rect.getHeight());
	setPosition(Vec2f(rect.x1, rect.y1));
	event = e;
}

SimpleSpriteButton::SimpleSpriteButton(float width, float height, const ci::Vec2f& position, EventGUIRef e)
	:Sprite(), color(Color::white()), bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
{
	buttonArea = Rectf(0.0f, 0.0f, width, height);
	setPosition(position);
	event = e;
}

SimpleSpriteButton::SimpleSpriteButton(const ci::Vec2f& position0, const ci::Vec2f& position)
	:Sprite(), color(Color::white()), bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
{
	buttonArea = Rectf(Vec2f::zero(), position0);
	setPosition(position);
	event = EventGUIRef(new EventGUI());
}

void SimpleSpriteButton::mouseUp(ci::app::MouseEvent &e)
{
	if (inButtonField(e.getPos()))
		Sprite::mouseUp(e);
}

bool SimpleSpriteButton::inButtonField(const ci::Vec2i& pos)
{
	return (buttonArea + getGlobalPosition()).contains(pos);
}

void SimpleSpriteButton::drawLayout()
{
	gl::color(ColorA(color.r, color.g, color.b, alpha));
	gl::drawSolidRect(buttonArea);
	gl::color(Color::white());
	Sprite::drawLayout();
}

Vec2f SimpleSpriteButton::getPosition() const
{
	return Vec2f(buttonArea.x1, buttonArea.y1);
}

float SimpleSpriteButton::getWidth() const
{
	return buttonArea.x2 - buttonArea.x1;
}

float SimpleSpriteButton::getHeight() const
{
	return buttonArea.y2 - buttonArea.y1;
}

ci::Vec2f SimpleSpriteButton::getSize() const
{
	return ci::Vec2f(getWidth(), getHeight());
}

ci::Rectf SimpleSpriteButton::getButtonArea() const
{
	return buttonArea;
}

void SimpleSpriteButton::mouseUpHandler(const ci::Vec2i& vec)
{
}

void SimpleSpriteButton::setButtonArea(const ci::Rectf& rectf)
{
	buttonArea = rectf;
}

void SimpleSpriteButton::setButtonArea1(const ci::Rectf& rect)
{
	buttonArea = Rectf(0, 0, rect.getWidth(), rect.getHeight());
	setPosition(Vec2f(rect.x1, rect.y1));
}

void SimpleSpriteButton::setColor(const ci::Color& color)
{
	this->color = color;
}

float SimpleSpriteButton::getAlpha() const
{
	return alpha;
}

void SimpleSpriteButton::setAlpha(float alpha)
{
	this->alpha = alpha;
	color = Utils::colorAlpha(color, alpha);
	Sprite::setAlpha(alpha);
}

void SimpleSpriteButton::showAnimate(float startA, float endA, float time, float delay)
{
	alpha = 0.0f;
	timeline().apply(&alpha, startA, endA, time, EaseInOutCubic()).delay(delay);
}

void SimpleSpriteButton::showPositionAnimate(const ci::Vec2f& endA, float time, float delay)
{
	animPosition = endA;
	timeline().apply(&animPosition, Vec2f::zero(), time, EaseOutCubic()).delay(delay);
}

void SimpleSpriteButton::stopAllTweens()
{
	animPosition.stop();
	alpha.stop();
}