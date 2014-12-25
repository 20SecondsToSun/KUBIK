#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;

namespace ImageFilter{

	class BlackWhiteFilter
	{
	public:		
		Surface process(Surface imageIn)
		{
			int r,g,b,corfinal;
			Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{		
					r = iter.r();
					g = iter.g();
					b = iter.b();
					corfinal = (int)((r*0.3)+(b*0.59)+(g*0.11));
					imageIn.setPixel(iter.getPos(), ci::ColorA(corfinal, corfinal, corfinal));
				}
			}

			return imageIn;

		}
	};

}// namespace HaoRan
