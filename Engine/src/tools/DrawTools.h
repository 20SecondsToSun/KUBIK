#pragma once

#include "cinder/CinderMath.h"
#include "Utils.h"

#define RED ci::Color(1.0f, 0.0f, 0.0)

namespace kubik
{		
	class DrawTools
	{

	public:
		static DrawTools& getInstance() 
		{ 
			static DrawTools drawtools; 
			return drawtools; 
		};

		void circleSlice(float x, float y, float radius, float lowAngle, float highAngle, bool closed, bool radians = false);
		ci::gl::Texture circleSliceTexture(float x, float y, float radius, float lowAngle, float highAngle, bool closed, bool radians = false);
		void drawDashedLine(float x1, float y1, float x2, float y2, float width, float space);
		void drawStrokedRect(const ci::Rectf& rect, float lineWidth);
	};

	inline DrawTools& drawtool() {return DrawTools::getInstance();};
}