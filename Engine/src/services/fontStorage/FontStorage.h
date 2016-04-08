#pragma once

#include "Types.h"

namespace kubik
{
	class FontStorage
	{
	public:
		static FontStorage& getInstance();

		void init();
		void addFontToLoadingQuery(const std::string& name, std::vector<int> fontSizes);
		void add(const std::string& name, int size);
		void addToSettingsDictionary(const std::string& key, std::shared_ptr<FontResource>  value);

		std::shared_ptr<FontResource> createFontResource(const std::string& path, float size, loadingType loadType = loadingType::FULL_PATH);
		std::string getFontsPath(const std::string& value);

		IResourceDictionary getResources() const;
		FontResourceDictionary getAll() const;

		ci::Font getFont1(const std::string& name);
		ci::Font getFont1(const std::string& name, int size);

	private:
		FontResourceDictionary fonts;
		IResourceDictionary resources;		
	};

	inline FontStorage&	fontStorage() { return FontStorage::getInstance(); };
	inline ci::Font getFont(const std::string& name) { return FontStorage::getInstance().getFont1(name); };
	inline ci::Font getFont(const std::string& name, int size) { return FontStorage::getInstance().getFont1(name, size); };
}