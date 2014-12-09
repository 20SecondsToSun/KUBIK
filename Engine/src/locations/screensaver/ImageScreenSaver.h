#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

class ImageScreenSaver:public IResourceScreenSaver
{

public:
	ImageScreenSaver(gl::Texture texture)
	{
		this->texture = texture;
	}

	void draw()
	{
		gl::draw(texture);
	}

	void start()
	{
		
	}

	void stop()
	{
	
	}

private:
	gl::Texture texture;
};