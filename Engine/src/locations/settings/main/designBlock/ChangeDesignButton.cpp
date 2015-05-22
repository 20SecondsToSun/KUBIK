#include "main/designBlock/ChangeDesignButton.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

ChangeDesignButton::ChangeDesignButton(const SixButtonOneData& data):ImageQuadroButton(data)
{
	event = ChangeDesignEventRef(new ChangeDesignEvent(data.getDesignItem()));
}

PhotoOverButton::PhotoOverButton(const SixButtonOneData& data):ImageQuadroButton(data)
{
	event = ChangePhotoOverDesignEventRef(new ChangePhotoOverDesignEvent(data.getDesignItem()));
}

PhotoCardStyleButton::PhotoCardStyleButton(const SixButtonOneData& data):ImageQuadroButton(data)
{
	event = ChangePhotoCardStyleDesignEventRef(new ChangePhotoCardStyleDesignEvent(data.getDesignItem()));
}

PhotoFilterPreviewButton::PhotoFilterPreviewButton(const SixButtonOneData& data):ImageQuadroButton(data)
{
	event = ChangePhotoFilterPreviewActiveEventRef(new ChangePhotoFilterPreviewActiveEvent(data.getDesignItem()));
}