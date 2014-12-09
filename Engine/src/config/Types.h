#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/qtime/QuickTime.h"
#include <stdint.h>

using namespace std;
using namespace ci;

namespace kubik
{
	enum SettingTypes
	{
		MENU = 10,
		TUNEUP = 20
	};

	enum gameId
	{
		FUNCES = 1,
		PHOTOBOOTH = 2,
		KOTOPOZA = 3
	};
	
	enum resourceType
	{
		IMAGE,
		VIDEO,
		FONT
	};

	enum loadingType
	{
		RESOURCE,
		ASSET,
		FULL_PATH,
		URL
	};

	class Types
	{	
	public:	

		typedef struct __tex
		{
			string path;
			ci::Surface tex;
			qtime::MovieGl movie;
			ci::Font font;
			bool isLoading;
			resourceType resourceType;
			loadingType  loadingType;
			float  fontSize;
		}
		TexObject;

		typedef map<string, TexObject *> OneBlockTexDictionary;		
	};	
}

template <typename Sig, typename F> void connect_once(Sig& sig, F&& f) 
{		
	if (!sig.num_slots()) 
		sig.connect(std::forward<F>(f));
}