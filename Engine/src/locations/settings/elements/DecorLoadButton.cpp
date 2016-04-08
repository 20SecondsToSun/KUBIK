#include "DecorLoadButton.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

DecorLoadButton::DecorLoadButton(const std::string& path,
	const ci::Rectf& rect,
	const TextItem& textItem,
	const ci::gl::Texture& icon, 
	const ci::gl::Texture& over)
	:LoadButton(path, rect, textItem, icon), over(over)
{

}

void DecorLoadButton::drawLayout()
{	
	LoadButton::drawLayout();

	gl::pushMatrices();
	gl::translate(-7, -207);
	gl::draw(over);
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