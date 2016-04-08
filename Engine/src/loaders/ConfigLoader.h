#pragma once

#include "IScreen.h"
#include "IGame.h"
#include "ServicePopup.h"
#include "preloader/Preloader.h"
#include "ILoader.h"

namespace kubik
{
	typedef std::shared_ptr<class ConfigLoader> ConfigLoaderRef;

	class ConfigLoader : public ILoader
	{
	public:	
		ConfigLoader(config::ApplicationModelRef model);

		void load();
		bool loadKubikConfig();
		void loadConfigs(std::list<config::ISettingsRef> configs);

	private:	
		config::ApplicationModelRef model;
	};
}
