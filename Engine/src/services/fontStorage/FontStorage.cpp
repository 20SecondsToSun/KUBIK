#include "fontStorage/FontStorage.h"
#include "Paths.h"

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
		vector<int> fontSizes{ 30, 24, 28, 44, 60, 36, 18, 90, 72 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "HelveticaLight";
		vector<int> fontSizes{ 24, 22, 20, 25, 36 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "Helvetica Neue";
		vector<int> fontSizes{ 90, 20 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "IntroBold";
		vector<int> fontSizes{ 60, 70, 72, 80, 90, 110 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "HypatiaSansPro-Black";
		vector<int> fontSizes{ 100 };
		addFontToLoadingQuery(fontName, fontSizes);
	}

	{
		string fontName = "Idealist-SC";
		vector<int> fontSizes{ 40 };
		addFontToLoadingQuery(fontName, fontSizes);
	}
}

void FontStorage::addFontToLoadingQuery(const std::string& fontName, std::vector<int> fontSizes)
{
	for (size_t i = 0; i < fontSizes.size(); i++)
		add(fontName, fontSizes[i]);
}

void FontStorage::add(const std::string& name, int size)
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

void FontStorage::addToSettingsDictionary(const std::string& key, shared_ptr<FontResource>  value)
{
	fonts[key] = value;
	resources[key] = value;
}

std::string FontStorage::getFontsPath(const std::string& value)
{
	return Paths::getFontsPath().string() + value;
}

IResourceDictionary FontStorage::getResources() const
{
	return resources;
}

FontResourceDictionary FontStorage::getAll() const
{
	return fonts;
}

Font FontStorage::getFont1(const std::string& name)
{
	return fonts[name]->get();
}

Font FontStorage::getFont1(const std::string& name, int size)
{
	return fonts[name + to_string(size)]->get();
}