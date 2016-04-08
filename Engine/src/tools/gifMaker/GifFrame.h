#pragma once

namespace kubik
{
	typedef struct
	{
		unsigned char * pixels;
		int width;
		int height;
		float duration;  
		int bitsPerPixel;
	} ofxGifFrame;
}
