#pragma once
#include "gui/Sprite.h"
#include "InstakubSettings.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class InstagramView> InstagramViewRef;

			class InstagramView : public Sprite
			{
				float yPosition;
			public:
				InstagramView();
				void draw();
				void setYPosition(float y);
			};
		}
	}
}