#include "HumanModel.h"

using namespace kubik::games::poza;

void HumanModel::calculateHeightScale()
{
	float x1 = MAX_USER_HEIGHT;
	float y1 = MIN_USER_DISTANCE;

	float x2 = MIN_USER_HEIGHT;
	float y2 = MAX_USER_DISTANCE;

	float del = (y2 - y1);

	distance = Utils::clamp(distance, MAX_USER_DISTANCE, MIN_USER_DISTANCE);

	if (del != 0)
	{
		etalonHeightAccordingDepth = ((x1 * y2 - x2 * y1) + (x2 - x1) * distance) / del;
	}
	else
	{
		etalonHeightAccordingDepth = MAX_USER_HEIGHT;
	}

	if (etalonHeightAccordingDepth != 0)
	{
		scaleAccordingUserHeight = height / etalonHeightAccordingDepth;
	}
	else
	{
		scaleAccordingUserHeight = 1;
	}

	scaleAccordingUserHeight = Utils::clamp(scaleAccordingUserHeight, 1.0f, 0.5f);
}