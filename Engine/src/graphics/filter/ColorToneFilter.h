#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;

#define SAFECOLOR(color) min(255.0, max(0.0, color))

namespace ImageFilter
{

class ColorToneFilter
{

private:
	double   _hue ;
    double   _saturation ;
    double   _lum_tab[256] ;

public:
	 /// @name RGB <--> HLS (Hue, Lightness, Saturation).
    //@{
    /**
        RGB --> HLS \n
        prgb - address of 24bpp or 32bpp pixel.
    */
    static void RGBtoHLS (Color rgb, double& H, double& L, double& S)
    {
        Color color = rgb;
        int   n_cmax = max(color.r, max(color.g, color.b));
        int   n_cmin = min(color.r, min(color.g, color.b));

        L = (n_cmax + n_cmin) / 2.0 / 255.0 ;

        if (n_cmax == n_cmin)
        {
            S = H = 0.0 ;
            return ;
        }

        double   r = color.r / 255.0,
                 g = color.g / 255.0,
                 b = color.b / 255.0,
                 cmax = n_cmax / 255.0,
                 cmin = n_cmin / 255.0,
                 delta = cmax - cmin ;

        if (L < 0.5) 
            S = delta / (cmax + cmin) ;
        else
            S = delta / (2.0 - cmax - cmin) ;

        if (color.r == n_cmax)
            H = (g-b) / delta ;
        else if (color.g == n_cmax)
            H = 2.0 + (b-r) / delta ;
        else
            H = 4.0 + (r-g) / delta ;

        H /= 6.0 ;

        if (H < 0.0)
            H += 1.0 ;
    }

	static Color DoubleRGB_to_RGB (double r, double g, double b)
    {
		return Color(SAFECOLOR(r*255), SAFECOLOR(g*255), SAFECOLOR(b*255)) ;
    }

	static double HLS_Value (double n1, double n2, double h)
    {
        if (h > 6.0)
            h -= 6.0 ;
        else if (h < 0.0)
            h += 6.0 ;

        if (h < 1.0)
            return n1 + (n2 - n1) * h ;
        else if (h < 3.0)
            return n2 ;
        else if (h < 4.0)
            return n1 + (n2 - n1) * (4.0 - h) ;
        return n1 ;
    }

    /// HLS --> RGB.
    static Color HLStoRGB (double H, double L, double S)
    {
        if ((!(S > 0)) && (!(S < 0))) // == 0
            return DoubleRGB_to_RGB (L, L, L) ;

        double   m1, m2 ;
        if (L > 0.5)
            m2 = L + S - L*S ;
        else
            m2 = L * (1.0 + S) ;
        m1 = 2.0*L - m2 ;

        double   r = HLS_Value (m1, m2, H*6.0 + 2.0) ;
        double   g = HLS_Value (m1, m2, H*6.0) ;
        double   b = HLS_Value (m1, m2, H*6.0 - 2.0) ;
        return DoubleRGB_to_RGB (r,g,b) ;
    }

	
	 /**
        Calculate grayscale value of pixel \n
        prgb - address of 24bpp or 32bpp pixel.
    */
    static int GetGrayscale (int r, int g , int b)
    {
        return (int)((30*r + 59*g + 11*g) / 100) ;
    }

	ColorToneFilter(Color tone, int saturation)
    {
        double   l ;
        RGBtoHLS (tone, _hue, l, _saturation) ;

        _saturation = _saturation * (saturation/255.0) * (saturation/255.0) ;
        _saturation = ((_saturation < 1) ? _saturation : 1) ;

        for (int i=0 ; i < 256 ; i++)
        {
            Color   cr(i,i,i);
            double    h, ll, s ;
            RGBtoHLS (cr, h, ll, s) ;

            ll = ll * (1 + (128-abs(saturation-128)) / 128.0 / 9.0) ;
            _lum_tab[i] = ((ll < 1) ? ll : 1) ;
        }
    };


	Surface process(Surface imageIn)
	{
		  int r, g, b;
		  Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{	
					r = iter.r();
					g = iter.g();
					b = iter.b();

				   double  l = _lum_tab[GetGrayscale(r, g, b)] ;
				   Color   cr = HLStoRGB (_hue, l, _saturation) ;
				   imageIn.setPixel(iter.getPos(), ci::ColorA(cr.r, cr.g, cr.b));
			  }
		  }
		return imageIn;
	}
};

}