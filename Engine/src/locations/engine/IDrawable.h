#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;

typedef struct __tex
{
	std::string path;
	ci::gl::Texture tex;
	bool isLoading;
}TexObject;

typedef map<string, TexObject *> OneBlockTexDictionary;

class IDrawable
{	
	
protected:
	OneBlockTexDictionary designTexures;

	void addToDictionary(string key, string path)
	{
		TexObject *value	= new TexObject();
		value->path			= path;
		value->isLoading	= false;
		value->tex			= ci::gl::Texture();

		designTexures[key] = value;
	}

public:

	virtual void draw() = 0;
	virtual void init() = 0;

	OneBlockTexDictionary getTextures()
	{	
		return designTexures;
	}

	void trace()
	{

	}
};