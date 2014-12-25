#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;

namespace ImageFilter
{

	class BrickFilter 
	{
	public:
		int ThreshHold;

		BrickFilter(): ThreshHold(12){};

		Surface process(Surface imageIn)
		{	  
			int r = 0, g = 0, b = 0, avg = 0;

			Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{	
					r = iter.r();
					g = iter.g();
					b = iter.b();
					avg = (r + g + b) / 3;
					avg = avg >= ThreshHold ? 255 : 0;
					imageIn.setPixel(iter.getPos(), ci::ColorA(avg, avg, avg));

				}
			} 

			return imageIn;     
		}
	};

}