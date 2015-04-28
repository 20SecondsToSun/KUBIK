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
				void load() override;
				void draw() override;
				void reset() override;				
				void timeOutReload() override;						
			};
		}
	}
}