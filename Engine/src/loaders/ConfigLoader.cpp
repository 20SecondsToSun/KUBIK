#include "ConfigLoader.h"

using namespace std;
using namespace ci;
using namespace kubik::config;
using namespace kubik;

ConfigLoader::ConfigLoader(ApplicationModelRef model)
{
	this->model = model;
}

void ConfigLoader::load()
{
	if (loadKubikConfig())
		LoadingCompleteSignal();
}

bool ConfigLoader::loadKubikConfig()
{
	bool success = false;

	try
	{
		model->load();
		success = true;
	}
	catch (ExcConfigFileParsing exc)
	{
		LoadingErrorSignal(exc);
	}
	catch (...)
	{
		ExcConfigFileParsing exc;
		LoadingErrorSignal(exc);
	}

	return success;
}

void ConfigLoader::loadConfigs(list<ISettingsRef> configs)
{
	for (auto conf : configs)
	{
		try
		{
			conf->load();
		}
		catch (ExcConfigFileParsing exc)
		{
			LoadingErrorSignal(exc);
			return;
		}
		catch (ExcBigFileSizeOfScreenSaver exc)
		{
			LoadingErrorSignal(exc);
			return;
		}
	}
}