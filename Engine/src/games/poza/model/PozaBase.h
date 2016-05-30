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
			class PozaBase
			{
				std::vector<kubik::config::PozaSettings::PozaDataObj> data;
				int index;

			public:
				PozaBase()
				{

				}

				PozaBase(const PozaBase& pb)
				{
					if (&pb != this)
					{
						data = pb.data;
						index = pb.index;
					}
				}

				void shuffleDataBase();
				kubik::config::PozaSettings::PozaDataObj getPose();
				kubik::config::PozaSettings::PozaDataObj getScaledPose(float scaleFactor)
				{
					auto poza = getPose();
					auto shift = poza.points[11];
					for (size_t i = 0; i < poza.points.size(); i++)
					{
						auto scaledCoords = (poza.points[i] - shift) * scaleFactor + shift;
						poza.points[i] = scaledCoords;
					}

					return poza;
				}

				friend class Poza;
				friend class PozaGame;
			};
		}
	}
}