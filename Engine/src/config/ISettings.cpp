
#include "ISettings.h"
using namespace kubik::config;
using namespace kubik;
using namespace std;

ISettings::ISettings(ApplicationModelRef model) : model(model)
{

}

std::string ISettings::getDesignPath() const
{
	return getBasePath().string() + designPath;
}

std::string ISettings::getStaticDesignPath(const string& value) const
{
	return getBasePath().string() + staticDesignPath + value;
}

std::string ISettings::getTemplateDesignPath(const string& value) const
{
	return getBasePath().string() + templateDesignPath + value;
}

std::string ISettings::getFontsPath(const string& value)
{
	ci::fs::path path = getBasePath() / "data\\fonts\\";
	return path.string() + value;
}

std::string ISettings::getInterfacePath(const string& value)
{
	ci::fs::path path = getBasePath() / "data\\interface\\";
	return path.string() + value;
}

ImageResourceDictionary ISettings::getTextures() const
{
	return textures;
}

Texture ISettings::getTexture(const string& name)
{
	return textures[name]->get();
}

FontResourceDictionary ISettings::getFonts() const
{
	return fonts;
}

Font ISettings::getFont(const string& name)
{
	return fonts[name]->get();
}

VideoResourceDictionary ISettings::getVideos() const
{
	return videos;
}

IResourceDictionary ISettings::getResources() const
{
	return resources;
}

IResourceDictionary ISettings::getSettingsResources() const
{
	return settingsResources;
}

void ISettings::clearResources()
{
	textures.clear();
	fonts.clear();
	videos.clear();
	resources.clear();
}

void ISettings::buildLocationData()
{
};

void ISettings::buildSettingData()
{
};

ci::fs::path ISettings::getBasePath()
{
	ci::fs::path basePath = ci::app::getAppPath();
	return basePath;
}

void ISettings::addToDictionary(const std::string& key, shared_ptr<ImageResource>  value)
{
	textures[key] = value;
	resources[key] = value;
}

void ISettings::addToDictionary(const std::string& key, shared_ptr<FontResource>  value)
{
	fonts[key] = value;
	resources[key] = value;
}

void ISettings::addToDictionary(const std::string& key, shared_ptr<VideoResource>  value)
{
	videos[key] = value;
	resources[key] = value;
}


void ISettings::addToSettingsDictionary(const std::string& key, shared_ptr<ImageResource>  value)
{
	textures[key] = value;
	settingsResources[key] = value;
}

void ISettings::addToSettingsDictionary(const std::string& key, shared_ptr<FontResource>  value)
{
	fonts[key] = value;
	settingsResources[key] = value;
}

void ISettings::addToSettingsDictionary(const std::string& key, shared_ptr<VideoResource>  value)
{
	videos[key] = value;
	settingsResources[key] = value;
}




shared_ptr<ImageResource> ISettings::createImageResource(const std::string& path, loadingType loadType)
{
	shared_ptr<ImageResource> value = shared_ptr<ImageResource>(new ImageResource);
	value->path = path;
	value->loadingType = loadType;

	return value;
}

shared_ptr<FontResource> ISettings::createFontResource(const std::string& path, float size, loadingType loadType)
{
	shared_ptr<FontResource> value = shared_ptr<FontResource>(new FontResource);
	value->path = path;
	value->fontSize = size;
	value->loadingType = loadType;

	return value;
}

shared_ptr<VideoResource> ISettings::createVideoResource(const std::string& path, loadingType loadType)
{
	shared_ptr<VideoResource> value = shared_ptr<VideoResource>(new VideoResource);
	value->path = path;
	value->loadingType = loadType;

	return value;
}