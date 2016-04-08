#include "ScreenStorage.h"

using namespace kubik;

void ScreenStorage::setScreenShot(const ci::gl::Texture& texture)
{
	this->texture = texture;
}

ci::gl::Texture ScreenStorage::getScreenShot() const
{
	return texture;
}

void ScreenStorage::drawScreenShot()
{
	if (texture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		ci::gl::draw(texture);
		ci::gl::enableAlphaBlending();
	}
}
	