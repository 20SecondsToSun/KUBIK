#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;

namespace ImageFilter{

	typedef unsigned char byte;

	class BrightContrastFilter
	{

	public:
		/// The brightness factor.
		/// Should be in the range [-1, 1].
		float BrightnessFactor;

		/// The contrast factor.
		/// Should be in the range [-1, 1].
		float ContrastFactor;

		BrightContrastFilter():BrightnessFactor(0.25f), ContrastFactor(0){};

		//@Override
		Surface process(Surface imageIn)
		{
			int r, g, b;
			// Convert to integer factors
			int bfi = (int)(BrightnessFactor * 255);
			float cf = 1 + ContrastFactor;
			cf *= cf;
			int cfi = (int)(cf * 32768) + 1;
			//Area area( 0, 0, 500, 320 );
			Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{			
					r = iter.r();
					g = iter.g();
					b = iter.b();
					// Modify brightness (addition)
					if (bfi != 0)
					{
						// Add brightness
						int ri = r + bfi;
						int gi = g + bfi;
						int bi = b + bfi;
						// Clamp to byte boundaries
						r = (byte)(ri > 255 ? 255 : (ri < 0 ? 0 : ri));
						g = (byte)(gi > 255 ? 255 : (gi < 0 ? 0 : gi));
						b = (byte)(bi > 255 ? 255 : (bi < 0 ? 0 : bi));
					}
					// Modifiy contrast (multiplication)
					if (cfi != 32769)
					{
						// Transform to range [-128, 127]
						int ri = r - 128;
						int gi = g - 128;
						int bi = b - 128;

						// Multiply contrast factor
						ri = (ri * cfi) >> 15;
						gi = (gi * cfi) >> 15;
						bi = (bi * cfi) >> 15;

						// Transform back to range [0, 255]
						ri = ri + 128;
						gi = gi + 128;
						bi = bi + 128;

						// Clamp to byte boundaries
						r = (byte)(ri > 255 ? 255 : (ri < 0 ? 0 : ri));
						g = (byte)(gi > 255 ? 255 : (gi < 0 ? 0 : gi));
						b = (byte)(bi > 255 ? 255 : (bi < 0 ? 0 : bi));
					}
					imageIn.setPixel(iter.getPos(), ci::ColorA(r, g, b));//setPixelColor(x, y, r, g, b);					
				}
			}
#ifndef WIN32 //only for apple ios
			imageIn.copyPixelsFromBuffer();
#endif
			return imageIn;
		}
	};
}// namespace HaoRan

