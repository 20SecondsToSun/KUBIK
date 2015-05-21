#include "main/designBlock/ChangeDesignButton.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

ChangeDesignButton::ChangeDesignButton(const OneDesignItem& item, const ci::gl::Texture& overtex, const Vec2f& pos)
	:ImageQuadroButton(item, overtex, pos)
{
	event = ChangeDesignEventRef(new ChangeDesignEvent(item));
}

PhotoOverButton::PhotoOverButton(const OneDesignItem& item, const ci::gl::Texture& overtex, const Vec2f& pos)
	: ImageQuadroButton(item, overtex, pos)
{
	event = ChangePhotoOverDesignEventRef(new ChangePhotoOverDesignEvent(item));
}

PhotoCardStyleButton::PhotoCardStyleButton(const OneDesignItem& item, const ci::gl::Texture& overtex, const Vec2f& pos)
	: ImageQuadroButton(item, overtex, pos)
{
	event = ChangePhotoCardStyleDesignEventRef(new ChangePhotoCardStyleDesignEvent(item));
}

PhotoFilterPreviewButton::PhotoFilterPreviewButton(const OneDesignItem& item, const ci::gl::Texture& overtex, const Vec2f& pos)
	: ImageQuadroButton(item, overtex, pos)
{
	event = ChangePhotoFilterPreviewActiveEventRef(new ChangePhotoFilterPreviewActiveEvent(item));
}