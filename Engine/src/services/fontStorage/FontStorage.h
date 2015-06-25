#pragma once
#include "Types.h"
namespace kubik
{
	class FontStorage
	{
	public:
		static FontStorage& getInstance();

		void init();
		void addFontToLoadingQuery(std::string fontName, std::vector<int> fontSizes);
		void add(std::string name, int size);
		void addToSettingsDictionary(const std::string& key, shared_ptr<FontResource>  value);

		shared_ptr<FontResource> createFontResource(const std::string& path, float size, loadingType loadType = loadingType::FULL_PATH);
		std::string getFontsPath(const std::string& value);

		IResourceDictionary getResources() const;
		FontResourceDictionary getAll() const;

		ci::Font getFont(const std::string& name);

	private:
		FontResourceDictionary fonts;
		IResourceDictionary resources;
		
	};

	inline FontStorage&	fontStorage() { return FontStorage::getInstance(); };
}