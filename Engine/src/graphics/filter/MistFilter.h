//#pragma once
//#include "cinder/app/AppNative.h"
//#include "cinder/gl/gl.h"
//#include "cinder/gl/Texture.h"
//
//using namespace ci;
//
//namespace ImageFilter{
//
//	class MistFilter
//	{
//
//	public:
//		Surface process(Surface imageIn)
//		{
//			int width = imageIn.getWidth();
//			int height = imageIn.getHeight();
//			Surface clone = imageIn.clone();
//			int r = 0, g = 0, b = 0;
//
//			while( iter.line() ) 
//			{
//				while(iter.pixel()) 
//				{
//					int k = NoiseFilter::getRandomInt(1, 123456);
//					
//					int dx = x + k % 8;
//					int dy = y + k % 8;
//					if(dx >= width) {
//						dx = width -1;   				  
//					}
//					if(dy >= height) {
//						dy = height -1;   				  
//					}
//					r = clone.getRComponent(dx, dy);
//					g = clone.getGComponent(dx, dy);
//					b = clone.getBComponent(dx, dy);
//					imageIn.setPixelColor(x, y, r, g, b);
//				}
//			} 
//			return imageIn;     
//		}
//	};
//
//}