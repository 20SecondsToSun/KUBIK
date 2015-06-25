#pragma once
#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"

namespace kubik
{
	typedef std::shared_ptr<class ImageButtonSprite> ImageButtonSpriteRef;

	class ImageButtonSprite : public SimpleSpriteButton
	{
	public:
		ImageButtonSprite(const ci::gl::Texture& image, const ci::Vec2f& pos = Vec2f::zero());

		void drawLayout() override;
		void changeTexture(const ci::gl::Texture& image);

	private:
		ci::gl::Texture image;
	};
}