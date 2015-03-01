#pragma once

#include "cinder/CinderMath.h"
#include "Utils.h"

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

		void circleSlice(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians = false)
		{
			int numCirclePts = 100;
			float circlePtsScaled[220];
			//if (!bSetupCircle) setupCircle();  

			// use smoothness, if requested:  
			//if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();  

			bool angleWrap = (lowAngle > highAngle); // are we doing the 0/360 wrap?  

			if(!radians)
			{  
				lowAngle  =  ci::toRadians(lowAngle);  
				highAngle =  ci::toRadians(highAngle);  
			}  

			int res = numCirclePts;  
			float angle = lowAngle;  
			float angleRange = ((!angleWrap)?(highAngle - lowAngle):(M_PI*2 - lowAngle + highAngle));  
			float angleAdder = angleRange / (float)res;  
			int k = 0;  


			for (int i = 0; i < numCirclePts; i++)
			{  
				circlePtsScaled[k]   = x + cos(angle) * radius;  
				circlePtsScaled[k+1] = y - sin(angle) * radius;  
				angle += angleAdder;  
				k+=2;  
			} 
			// we draw the circle points ourself (vs. glDrawArrays) because it allows us to draw the center point, and have the triangles fan around it  
			k = 0;  
			//glBegin((drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : (closed?GL_LINE_LOOP:GL_LINE_STRIP));  
			glBegin(GL_TRIANGLE_FAN);			
			glVertex2f(x, y); // center vertex  

			// now all the points around the circumference  
			for (int i = 0; i < numCirclePts; i++)
			{  
				glVertex2f(circlePtsScaled[k], circlePtsScaled[k+1]);  
				k+=2; 
			}
			glEnd();
		}

		gl::Texture circleSliceTexture(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians = false)
		{
			gl::Fbo mFbo = gl::Fbo(radius * 2, radius * 2);
			Utils::drawGraphicsToFBO(mFbo, [ & ]()
			{
				gl::pushMatrices();
				gl::clear(Color::black());
				gl::enableAlphaBlending();  
				circleSlice(radius, radius, radius, lowAngle, highAngle, closed, radians);
				gl::popMatrices();
			});	
			gl::Texture tex = mFbo.getTexture();
			Utils::clearFBO(mFbo);
			return tex;
		}

	};
	// helper function(s) for easier access 
	inline DrawTools& drawtool() {return DrawTools::getInstance();};
}