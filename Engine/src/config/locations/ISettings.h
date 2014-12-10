#pragma once

#include "cinder/app/AppNative.h"
#include "Types.h"
#include "cinder/Json.h"
#include "ApplicationModel.h"

namespace kubik
{
	class ISettings
	{
	public:

		std::string getDesignPath()
		{
			return getBasePath().string() + designPath;
		}

		std::string getStaticDesignPath()
		{
			return getBasePath().string() + staticDesignPath;
		}

		std::string getTemplateDesignPath()
		{
			return getBasePath().string() + templateDesignPath;
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

		virtual void load() = 0;
		virtual void setTextures() = 0;	

	protected:		
		shared_ptr<ApplicationModel> model;	

		std::string configPath;

		std::string designPath;
		std::string staticDesignPath;
		std::string templateDesignPath;

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