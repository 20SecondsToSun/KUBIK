#include "elements/FilterButton.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::games::photobooth;
using namespace shaders::imagefilters;

FilterButton::FilterButton(int filterId, ci::Rectf rectf, int sizeID)
	:SimpleSpriteButton(rectf, PhotoFilterEventRef(new PhotoFilterEvent(filterId))), sizeID(sizeID),
	filterId(filterId)
{		
	maskShader = shadertool().getMaskShader();
}		

void FilterButton::drawLayout()
{		
	gl::color(color);	
	auto shader = shadertool().get((ShaderTool::FilterType)filterId);

	if(photoTex)
	{
		auto photo = Utils::drawGraphicsToFBO(photoTex.getSize(), [&]()
		{
			shader->render(photoTex);
		});

		auto photoMask = Utils::drawGraphicsToFBO(photoTex.getSize(), [&]()
		{
			gl::drawSolidCircle(Vec2f(0, 0), maskSize);
		});		

		maskShader->render(photo, photoMask);	
	}
}

void FilterButton::showAnimate(float time, float delay)
{
	maskSizeFinal = 1434.0f;
	maskSize = maskSizeInit = 1.0f;

	timeline().apply(&maskSize, maskSizeInit, maskSizeFinal, time,  EaseOutCubic()).delay(delay);
}

void FilterButton::setSizeID(int id)
{
	sizeID = id;
}

int FilterButton::getSizeID() const
{
	return sizeID;
}

void FilterButton::setTexture(const ci::gl::Texture& photoTex)
{
	this->photoTex = photoTex;

	auto h = photoTex.getHeight();
	auto w = photoTex.getWidth();
}

void FilterButton::setScale(float scale)
{
	this->scale = scale;
}