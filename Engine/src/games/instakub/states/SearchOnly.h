#pragma once
#include "instakub/states/SearchLocation.h"
#include "KeyBackground.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class SearchOnly> SearchOnlyRef;

			class SearchOnly : public SearchLocation
			{			
			public:
				SearchOnly(InstakubSettingsRef settings);
				virtual void loadStrategity() override;
				virtual void start() override;
				virtual void load() override;
				virtual void draw() override;
				virtual void reset() override;
				virtual void reload() override;
			};
		}
	}
}