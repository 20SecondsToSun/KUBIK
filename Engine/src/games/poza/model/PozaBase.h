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

				friend class Poza;
				friend class PozaGame;
			};
		}
	}
}