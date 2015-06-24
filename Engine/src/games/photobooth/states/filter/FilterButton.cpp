#include "states/filter/FilterButton.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::games::photobooth;
using namespace shaders::imagefilters;

FilterButton::FilterButton(int filterId, const ci::Rectf& rectf, int sizeID)
	:SimpleSpriteButton(rectf, PhotoFilterEventRef(new PhotoFilterEvent(filterId))),
	sizeID(sizeID),	filterId(filterId)
{		
	shader = shadertool().get((ShaderTool::FilterType)filterId);
}		

void FilterButton::drawLayout()
{		
	gl::color(color);	

	if(photoTex)
	{
		auto photo = Utils::drawGraphicsToFBO(photoTex.getSize(), [&]()
		{
			shader->render(photoTex);
		});

		gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
		gl::draw(photo);
		gl::color(Color::white());
	}
}

void FilterButton::setSizeID(int sizeID)
{
	this->sizeID = sizeID;
}

int FilterButton::getSizeID() const
{
	return sizeID;
}

void FilterButton::setTexture(const ci::gl::Texture& photoTex)
{
	this->photoTex = photoTex;
}

void FilterButton::setScale(float scale)
{
	this->scale = scale;
}