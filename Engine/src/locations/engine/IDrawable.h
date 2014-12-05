#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Types.h"

using namespace std;
using namespace kubik;
using namespace ci::signals;

class IDrawable
{	
	
protected:
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

public:

	virtual void draw() = 0;
	virtual void init() = 0;
	signal<void(void)> closeLocationSignal;

	Types::OneBlockTexDictionary getTextures()
	{	
		return designTexures;
	}

	void trace()
	{

	}
};