#pragma once
#include "instakub/states/InstakubLocation.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class SearchOnly> SearchOnlyRef;

			class SearchOnly : public InstakubLocation
			{
				ci::gl::Texture searchField;

			public:
				SearchOnly(InstakubSettingsRef settings);

				virtual void start();
				virtual void draw();
				virtual void reset();
				virtual void stop(){};
			};
		}
	}
}