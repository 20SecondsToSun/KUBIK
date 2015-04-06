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

				virtual void stop();
				virtual void start();
				virtual void load();
				virtual void draw();
				virtual void reset();
			};
		}
	}
}