#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;



namespace ImageFilter
{

class ReliefFilter
{

public:

	ReliefFilter(){};

	Surface process(Surface imageIn)
	{
		
		  Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()+1) 
				{	
				// int rr = iter.r - imageIn.getRComponent(x + 1, y) + 128;
				// int gg = imageIn.getGComponent(x, y) - imageIn.getGComponent(x + 1, y) + 128; 
				// int bb = imageIn.getBComponent(x, y) - imageIn.getBComponent(x + 1, y) + 128;
		        //´¦ÀíÒç³ö
		        if (rr > 255) rr = 255; 
		        if (rr < 0) rr = 0; 
		        if (gg > 255) gg = 255;
		        if (gg < 0) gg = 0;
		        if (bb > 255) bb = 255;
		        if (bb < 0) bb = 0;  
		        
		        imageIn.setPixel(iter.getPos(), ci::ColorA(rr, gg, bb));
	         }
        } 

        return imageIn;
	}
};

}