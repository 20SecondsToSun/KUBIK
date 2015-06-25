#include "fontStorage/FontStorage.h"

using namespace kubik;
using namespace std;
using namespace ci;

FontStorage& FontStorage::getInstance()
{
	static FontStorage fStorage;
	return fStorage;
};

void FontStorage::init()
{
	{
		string fontName = "Helvetica Neue";
		vector<int> fontSizes{ 24, 30 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "Helvetica Neue Light";
		vector<int> fontSizes{ 24, 26 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "Intro-Book";
		vector<int> fontSizes{ 44, 30, 36, 12, 14 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "IntroLight";
		vector<int> fontSizes{ 30, 24, 28, 44, 60, 36, 18, 90 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	addToSettingsDictionary("HelveticaLight24", createFontResource(getFontsPath("HelveticaLight.ttf"), 24));
	addToSettingsDictionary("HypatiaSansPro-Black100", createFontResource(getFontsPath("HypatiaSansPro-Black.ttf"), 100));
	addToSettingsDictionary("Idealist-SC40", createFontResource(getFontsPath("Idealist-SC.ttf"), 40));
}

void  FontStorage::addFontToLoadingQuery(std::string fontName, std::vector<int> fontSizes)
{
	for (size_t i = 0; i < fontSizes.size(); i++)
		add(fontName, fontSizes[i]);
}

void  FontStorage::add(std::string name, int size)
{
	addToSettingsDictionary(name + to_string(size), createFontResource(getFontsPath(name + ".ttf"), size));
}

shared_ptr<FontResource> FontStorage::createFontResource(const std::string& path, float size, loadingType loadType)
{
	shared_ptr<FontResource> value = shared_ptr<FontResource>(new FontResource);
	value->path = path;
	value->fontSize = size;
	value->loadingType = loadType;

	return value;
}

void  FontStorage::addToSettingsDictionary(const std::string& key, shared_ptr<FontResource>  value)
{
	fonts[key] = value;
	resources[key] = value;
}

std::string  FontStorage::getFontsPath(const std::string& value)
{
	ci::fs::path path = ci::app::getAppPath() / "data\\fonts\\";
	return path.string() + value;
}

IResourceDictionary FontStorage::getResources() const
{
	return resources;
}

FontResourceDictionary FontStorage::getAll() const
{
	return fonts;
}

Font  FontStorage::getFont(const std::string& name)
{
	return fonts[name]->get();
}