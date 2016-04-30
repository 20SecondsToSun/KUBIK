#include "instagram/ImageGraphic.h"

using namespace instagram;
using namespace ci;
using namespace ci::app;
using namespace std;

ImageGraphic::ImageGraphic()
	:animTime(.7f),
	animFunc(ci::EaseOutQuad()),
	loadedthumb(false)
{

}

void ImageGraphic::fadeIn(float from, float to)
{
	ci::app::timeline().apply(&alpha, from, to, animTime, animFunc);
}

float ImageGraphic::getAlpha() const
{
	return alpha;
}

void ImageGraphic::setSize(int size)
{
	width = height = size;
}

void ImageGraphic::draw(const ci::Vec2f& vec)
{
	if (lowTex)
	{
		gl::pushMatrices();
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
		float scale = ((float)width) / lowTex.getWidth();
		
		gl::translate(vec);
		gl::scale(scale, scale);
		gl::draw(lowTex);
		gl::popMatrices();		
	}
	else
	{
		lowTex = ph::fetchTexture(lowResURL);
		if (lowTex)
		{
			fadeIn(0.0f, 1.0f);
		}

		gl::pushMatrices();
		gl::translate(vec);
		gl::color(ColorA(1.f, 1.f, 1.f, 0.01f));
		gl::drawSolidRect(Rectf(5.0f, 5.0f, width - 5.0f, height - 5.0f));
		gl::popMatrices();
	}
}

void ImageGraphic::setLowResURL(const std::string& url)
{
	lowResURL = url;
}

std::string ImageGraphic::getLowResURL() const
{
	return lowResURL;
}

void ImageGraphic::setStandartResURL(const std::string& url)
{
	standartResURL = url;
}

std::string ImageGraphic::getStandartResURL() const
{
	return standartResURL;
}

ci::gl::Texture ImageGraphic::getStandartResImage()
{
	if (!bigtex)
	{
		bigtex = ph::fetchTexture(standartResURL);
	}

	return bigtex;
}


ci::gl::Texture ImageGraphic::getLowResImage()
{
	if (!lowTex)
	{
		lowTex = ph::fetchTexture(lowResURL);
	}

	return lowTex;
}