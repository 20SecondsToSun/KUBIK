#include "states/choosingButtons/FilterSmallButton.h"

using namespace kubik;
using namespace ci;
using namespace std;

FilterSmallButton::FilterSmallButton(const ci::Vec2f& vec, int id, const std::string &text, ci::Font fontC, ci::Font fontO)
	:SimpleSpriteButton(ci::Rectf(vec, vec + ci::Vec2f(109.0f, 228.0f)), FilterChangedEventRef(new FilterChangedEvent(id))),
	isSelected(false),
	id(id),
	text(text),
	fontC(fontC),
	fontO(fontO),
	state(CLOSE)
{
	boost::to_upper(this->text);

	titleSmall = textTools().getTextField(this->text, &fontC, Color::white()); 
	titleSmallPos = Vec2f((109 - titleSmall.getWidth()) * 0.5, 191);

	titleBig = textTools().getTextField(this->text, &fontO, Color::white()); 
	titleBigPos = Vec2f((137.0f - titleBig.getWidth()) * 0.5f, 211.0f);
}

void FilterSmallButton::setSelected(bool value)
{
	isSelected = value;
}

bool FilterSmallButton::selected()
{
	return isSelected;
}

void FilterSmallButton::drawLayout()
{
	if (state == CLOSE)
	{
		gl::color(Color::hex(0x191b1c));
		gl::drawSolidRect(buttonArea);
		gl::color(Color::white());
		gl::drawSolidRect(Rectf(0.0f, 0.0f, getWidth(), 140.0f));	
		gl::draw(titleSmall, titleSmallPos);
		
	}
	else
	{
		gl::color(Color::hex(0x191b1c));
		gl::drawSolidRect(buttonArea);
		gl::color(Color::white());
		gl::drawSolidRect(Rectf(0.0f, -30.0f, getWidth(), 147.0f));	
		gl::draw(titleBig, titleBigPos);
	}
}

int FilterSmallButton::getID() const
{
	return id;
}

void FilterSmallButton::setOpenState()
{
	state = OPEN;
	buttonArea = Rectf(Vec2f(0.0f, -30.0f), Vec2f(137.0f, 256.0f));
}

void FilterSmallButton::setCloseState()
{
	state = CLOSE;
	buttonArea = Rectf(Vec2f::zero(), Vec2f(109.0f, 228.0f));
}