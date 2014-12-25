﻿#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;

namespace ImageFilter
{
	class AutoLevelFilter  
	{
	private:
		vector<float> ComputeGamma(vector<int> lo, vector<int> md, vector<int> hi)
		{
			vector<float> _array(3);
			for (int i = 0; i < 3; i++){
				if (lo[i] < md[i] && md[i] < hi[i]) {
					double gamma = log(/*0.5, */(double) (((float) (md[i] - lo[i])) / ((float) (hi[i] - lo[i]))));
					_array[i] = (gamma > 10.0) ? ((float) 10.0) : ((gamma < 0.1) ? ((float) 0.1) : ((float) gamma));
				}
				else{
					_array[i] = 1;
				}
			}
			return _array;
		}

		vector<int> GetMeanColor(int h[3][256])
		{
			vector<int> _array(3);
			for (int i = 0; i < 3; i++) {
				long sum1 = 0L;
				long sum2 = 0L;
				for (int j = 0; j < 256; j++){
					sum1 += j * h[i][j];
					sum2 += h[i][j];
				}
				_array[i] = (sum2 == 0L) ? 0 : (((float) sum1) / ((float) sum2));
			}
			_array[0] = (((int) (_array[0] + 0.5f)) & 255);
			_array[1] = (((int) (_array[1] + 0.5f)) & 255);
			_array[2] = (((int) (_array[2] + 0.5f)) & 255);
			return _array;
		}

		vector<int> GetPercentileColor(int h[3][256], float fraction)
		{
			vector<int> _array(3);
			for (int i = 0; i < 3; i++)
			{
				long sum1 = 0L;
				long sum2 = 0L;
				for (int j = 0; j < 256; j++) {
					sum2 += h[i][j];
				}
				for (int k = 0; k < 256; k++) {
					sum1 += h[i][k];
					if (sum1 > (sum2 * fraction)) {
						_array[i] = k;
						break;
					}
				}
			}
			return _array;
		}

	public:
		float Intensity;
		AutoLevelFilter(float intensity):Intensity(intensity){}
		Surface process(Surface imageIn)
		{
			int h[3][256];
			int _array[3];
			int rgb[3] = { 255, 255, 255 };
			int bb[256];
			int gg[256];
			int rr[256];
			int intensity = (int)(Intensity * 255);
			int intensity_invert = 255 - intensity;
			Surface::Iter iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{	
					h[0][iter.r()]++;
					h[1][iter.g()]++;
					h[2][iter.b()]++;
				}
			}
			vector<int> percentileColor = GetPercentileColor(h, 0.005f);
			vector<int> meanColor = GetMeanColor(h);
			vector<int> hi = GetPercentileColor(h, 0.995f);
			vector<float> gamma = ComputeGamma(percentileColor, meanColor, hi);
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 256; j++){
					int arr[3];
					for (int n = 0; n < 3; n++){
						float percent = j - percentileColor[n];
						if (percent < 0){
							arr[n] = _array[n];
						}
						else if ((percent + percentileColor[n]) >= hi[n]){
							arr[n] = rgb[n];
						}
						else {
							double adjust = _array[n] + ((rgb[n] - _array[n]) * pow((double) (percent / ((float) (hi[n] - percentileColor[n]))), (double) gamma[n]));
							arr[n] = (adjust > 255.0) ? ((int) 255.0) : ((adjust < 0.0) ? ((int) 0.0) : ((int) adjust));
						}
					}
					rr[j] = arr[0];
					gg[j] = arr[1];
					bb[j] = arr[2];
				}
			}
			Surface clone = imageIn.clone();
			int r,g,b;
			 iter = imageIn.getIter();
			while( iter.line() ) 
			{
				while(iter.pixel()) 
				{	
					r = iter.r();
					g = iter.g();
					b = iter.b();
					r = (r * intensity_invert + rr[r] * intensity) >> 8;
					g = (g * intensity_invert + gg[g] * intensity) >> 8;
					b = (b * intensity_invert + bb[b] * intensity) >> 8;
					imageIn.setPixel(iter.getPos(), ci::ColorA(r, g, b));
				}
			}

			return imageIn;//×öÖ±·½Í¼Ä£Ê½ÔöÇ¿
		}
	};
}