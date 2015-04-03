#include "instagram/ImageGraphic.h"

using namespace instagram;
using namespace ci;
using namespace std;

ImageGraphic::ImageGraphic() : animTime(.7f), animFunc(ci::EaseOutQuad()), loaded(false)
{

}

void ImageGraphic::setLowResSurface(const Surface& surf)
{
	lowSurface = surf;
}

ci::Surface ImageGraphic::getLowResSurface()
{
	return lowSurface;
}

void ImageGraphic::fadeIn(float from, float to)
{
	alpha = from;
	ci::app::timeline().apply(&alpha, to, animTime, animFunc);
}

float ImageGraphic::getAlpha() const 
{
	return alpha;
}

void ImageGraphic::setSize(int size)
{
	width = height = size;
}

void ImageGraphic::draw(const Vec2f& vec)
{
	if (loaded)
	{
		gl::pushMatrices();
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
		float scale = ((float)width) / lowSurface.getWidth();
		gl::translate(vec);
		gl::scale(scale, scale);
		gl::draw(lowSurface);
		gl::popMatrices();
	}
	else
	{
		gl::pushMatrices();
		gl::translate(vec);
		gl::color(ColorA(0.1f, 0.1f, 0.1f, 1.0f));
		gl::drawSolidRect(Rectf(0.0f, 0.0f, width - 0.0f, height - 0.0f));
		gl::popMatrices();
	}
}

void ImageGraphic::drawStandartResImage(const Vec2f& vec)
{
	using namespace ci;

	if (standartSurface)
	{
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
		gl::draw(standartSurface, vec);
	}
}

void ImageGraphic::setLowResURL(const string& url)
{
	lowResURL = url;
}

string ImageGraphic::getLowResURL() const 
{
	return lowResURL;
}

void ImageGraphic::setStandartResURL(const string& url)
{
	standartResURL = url;
}

void ImageGraphic::loadLowRes()
{
	if (loaded)
		return;

	ci::app::console() << lowResURL << std::endl;
	try
	{
		lowSurface = loadImage(loadUrl(lowResURL));
		ci::app::console() << "loaded" << std::endl;
		loaded = true;		
	}
	catch (...)
	{
	}

	fadeIn(0, 1);
}

void ImageGraphic::setResImage(const Surface& surf)
{
	lowSurface = surf;
	fadeIn(0, 1);
}


void ImageGraphic::loadStandartRes()
{
	if (standartSurfaceTex)
		return;

	try
	{
		standartSurface = loadImage(loadUrl(standartResURL));
		standartSurfaceTex = gl::Texture(standartSurface);
	}
	catch (...)
	{
	}
}

gl::Texture ImageGraphic::getStandartResImage()
{
	if (standartSurface)
		return standartSurface;

	return gl::Texture();
}