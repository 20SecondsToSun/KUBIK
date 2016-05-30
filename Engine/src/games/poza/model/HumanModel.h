#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "PozaSettings.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			class HumanModel
			{
				const float   MAX_USER_DISTANCE = 3.5f;
				const float	  MIN_USER_DISTANCE = 2.6f;
				const float   MAX_USER_HEIGHT	= 325.0f;
				const float   MIN_USER_HEIGHT	= 240.0f;

			public:
				void calculateHeightScale();
				float height;
				float distance;
				float etalonHeightAccordingDepth;
				float scaleAccordingUserHeight;	
			};
		}
	}
}