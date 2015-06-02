#include "PreloaderSettings.h"

using namespace kubik;
using namespace kubik::config;

PreloaderSettings::PreloaderSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	configSettings(configSettings)
{

}

void PreloaderSettings::load()
{
	mainConfigObj = model->getConfigObject(settings::id::PRELOADERS);
	
	try
	{
		loadPaths();
	}
	catch (...)
	{
		throw ExcConfigFileParsing();
	}

	setTextures();
}

void PreloaderSettings::loadPaths()
{
	JsonTree pathJSON = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	finalPath = pathJSON.getChild("finalPath").getValue<string>();
}

void PreloaderSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + finalPath;
}

void PreloaderSettings::setTextures()
{
	setDesignPath();
	clearResources();

	auto addPreloaderFilesToDictionary = [&](MovieLoader::MovieLoaderStruct& movieStruct, const string& name)
	{		movieStruct = movieLoader().getMovieStruct(getTemplateDesignPath(name + "\\"), name);	

		if (movieStruct.type == MovieLoader::VIDEO)
			addToDictionary(movieStruct.name, createVideoResource(movieStruct.paths[0]));
		else if (movieStruct.type == MovieLoader::IMAGE_SEQUENCE)
			for (size_t i = 0; i < movieStruct.paths.size(); i++)
				addToDictionary(movieStruct.name + to_string(i), createImageResource(movieStruct.paths[i]));
	};

	addPreloaderFilesToDictionary(mainPreloaderStruct, "mainpreloader");
	addPreloaderFilesToDictionary(miniPreloaderStruct, "minipreloader");
}

void PreloaderSettings::buildData()
{
	mainPreloader = movieLoader().getMovie(mainPreloaderStruct, getTextures(), getVideos());
	miniPreloader = movieLoader().getMovie(miniPreloaderStruct, getTextures(), getVideos());
}

changeSetting::id PreloaderSettings::getChangeID() const
{
	return changeSetting::id::PRELOADER;
}

IMovieRef PreloaderSettings::getMainPreloader() const
{
	return mainPreloader;
}

IMovieRef PreloaderSettings::getMiniPreloader() const
{
	return miniPreloader;
}