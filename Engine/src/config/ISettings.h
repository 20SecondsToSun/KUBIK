#pragma once

#include "cinder/app/AppNative.h"
#include "Types.h"

namespace kubik
{
	class ISettings
	{
	public:

		std::string getDesignPath()
		{
			return getBasePath().string() + designPath;
		}

		static std::string getFontsPath()
		{
			ci::fs::path path = getBasePath()/"data/fonts/";
			return path.string();
		}

		Types::OneBlockTexDictionary getTextures()
		{	
			return designTexures;
		}

	protected:	

		std::string designPath;
		std::string fontsPath;

		static ci::fs::path getBasePath()
		{
			ci::fs::path basePath = ci::app::getAppPath();
			return basePath;
		}	

		Types::OneBlockTexDictionary designTexures;

		void addToDictionary(string key, string path, resourceType resType = resourceType::IMAGE, loadingType loadType = loadingType::FULL_PATH, float fontSize = 30)
		{
			Types::TexObject *value	= new Types::TexObject();
			value->path				= path;
			value->isLoading		= false;
			value->tex				= ci::Surface();

			value->resourceType				= resType;
			value->loadingType				= loadType;
			value->fontSize					= fontSize;

			designTexures[key] = value;
		}
	};
}