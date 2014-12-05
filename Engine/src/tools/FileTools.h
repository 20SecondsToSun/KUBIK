#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

using namespace ci;

class FileTools
{
public:

	static FileTools& getInstance() { 
		static FileTools strt; 
		return strt; 
	};

	std::ifstream::pos_type filesize(const char* filename)
	{
		std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
		return in.tellg(); 
	}
};

// helper function(s) for easier access 
inline FileTools&	fileTools() { return FileTools::getInstance(); };