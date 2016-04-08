#include "LoadButton.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

LoadButton::LoadButton(const std::string &path, const ci::Rectf& rect, const TextItem& textItem, const ci::gl::Texture& icon)
	:SimpleSpriteButton(rect, OpenSystemDirectoryEventRef(new OpenSystemDirectoryEvent(path))),
	textItem(textItem), 
	icon(icon), 
	bckColor(Color::hex(0x242135))
{
	tex = textTools().getTextField(textItem);
	texPosX = 0.5 * (buttonArea.getWidth() - (tex.getWidth() + icon.getWidth() + 15.0f));
}

void LoadButton::drawLayout()
{	
	gl::color(bckColor);
	gl::drawSolidRoundedRect(buttonArea, 7);
	gl::color(Color::white());				

	gl::pushMatrices();
	gl::translate(texPosX, 23);				
	gl::draw(tex);
	gl::translate(tex.getWidth() + 15.0f, 0);
	gl::translate(0, -5);	
	gl::draw(icon);	
	gl::popMatrices();
}

void LoadButton::draw(float x, float y)
{
	gl::pushMatrices();
	gl::translate(x, y);
	draw();
	gl::popMatrices();
}

void LoadButton::draw()
{					
	gl::pushMatrices();				
	gl::translate(getGlobalPosition());
	drawLayout();
	gl::popMatrices();
}

void LoadButton::setAlpha(float  alpha)
{
	//color = Utils::colorAlpha(color, alpha);
	//bckColor = Utils::colorAlpha(bckColor, alpha);
}	
