#pragma once

#include "IScreen.h"
#include "IGame.h"
#include "ServicePopup.h"
#include "preloader/Preloader.h"
#include "ILoader.h"

using namespace std;
using namespace ci;
using namespace kubik::config;

namespace kubik
{
	class ConfigLoader : public ILoader
	{

	public:	
		ConfigLoader(ApplicationModelRef model)
		{
			this->model = model;
		}

		void load()
		{		
			if (loadKubikConfig())
				LoadingCompleteSignal();
		}	

		bool loadKubikConfig()
		{
			bool success = false;

			try
			{		
				model->load();	
				success = true;
			}
			catch(ExcConfigFileParsing exc)
			{
				LoadingErrorSignal(exc);
			}
			catch(...)
			{
				ExcConfigFileParsing exc;
				LoadingErrorSignal(exc);
			}

			return success;
		}

		void loadConfigs(list<ISettingsRef> configs)
		{
			for (auto conf : configs)
			{
				try
				{			
					conf->load();
				}
				catch(ExcConfigFileParsing exc)
				{
					LoadingErrorSignal(exc);
					return;
				}
				catch(ExcBigFileSizeOfScreenSaver exc)
				{
					LoadingErrorSignal(exc);
					return;
				}				
			}

			LoadingCompleteSignal();
		}

	private:	
		ApplicationModelRef model;		
	};

	typedef shared_ptr<ConfigLoader> ConfigLoaderRef;
}