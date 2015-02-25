#include "DecorLoadButton.h"

using namespace kubik;
using namespace kubik::config;

DecorLoadButton::DecorLoadButton(const std::string& path, const ci::Rectf& rect, const TextItem& textItem, const ci::gl::Texture& icon)
	:LoadButton(path, rect, textItem, icon)
{

}

void DecorLoadButton::drawLayout()
{	
	LoadButton::drawLayout();

	gl::pushMatrices();
	gl::translate(0.0f, -200.0f);
	gl::color(Color::hex(0xffff00));
	gl::lineWidth(7);
	gl::drawStrokedRoundedRect(ci::Rectf(ci::Vec2f::zero(), Vec2f(200.0f, 200.0f)), 6.0f);
	gl::lineWidth(1);	
	gl::popMatrices();
}

void DecorLoadButton::draw(float x, float y)
{
	LoadButton::draw(x, y);
}

void DecorLoadButton::draw()
{					
	LoadButton::draw();
}	