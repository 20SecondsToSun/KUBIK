#pragma once
#include "instakub/states/SearchLocation.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class HashtagAndSearch> HashtagAndSearchRef;

			class HashtagAndSearch : public SearchLocation
			{

			public:	
				HashtagAndSearch(InstakubSettingsRef settings);
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