#include "DrawTools.h"

using namespace ci;
using namespace kubik;
using namespace std;

void DrawTools::circleSlice(float x, float y, float radius, float lowAngle, float highAngle, bool closed, bool radians)
{
	int numCirclePts = 100;
	float circlePtsScaled[220];
	//if (!bSetupCircle) setupCircle();  

	// use smoothness, if requested:  
	//if (bSmoothHinted && drawMode == OF_OUTLINE) startSmoothing();  

	bool angleWrap = (lowAngle > highAngle); // are we doing the 0/360 wrap?  

	if (!radians)
	{
		lowAngle = ci::toRadians(lowAngle);
		highAngle = ci::toRadians(highAngle);
	}

	int res = numCirclePts;
	float angle = lowAngle;
	float angleRange = ((!angleWrap) ? (highAngle - lowAngle) : (M_PI * 2 - lowAngle + highAngle));
	float angleAdder = angleRange / (float)res;
	int k = 0;

	for (int i = 0; i < numCirclePts; i++)
	{
		circlePtsScaled[k] = x + cos(angle) * radius;
		circlePtsScaled[k + 1] = y - sin(angle) * radius;
		angle += angleAdder;
		k += 2;
	}
	// we draw the circle points ourself (vs. glDrawArrays) because it allows us to draw the center point, and have the triangles fan around it  
	k = 0;
	//glBegin((drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : (closed?GL_LINE_LOOP:GL_LINE_STRIP));  
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center vertex  

	// now all the points around the circumference  
	for (int i = 0; i < numCirclePts; i++)
	{
		glVertex2f(circlePtsScaled[k], circlePtsScaled[k + 1]);
		k += 2;
	}
	glEnd();
}

gl::Texture DrawTools::circleSliceTexture(float x, float y, float radius, float lowAngle, float highAngle, bool closed, bool radians)
{
	gl::Fbo mFbo = gl::Fbo(radius * 2, radius * 2);
	Utils::drawGraphicsToFBO(mFbo, [&]()
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

//glLineStipple(1, 0x0101);		// точечный	
//glLineStipple(1, 0x00F0);		// штриховой	
//glLineStipple(1, 0x1C47);		// штрихпунктир
//glLineStipple(1, 0x1C47);		// тире, точка, тире

void DrawTools::drawDashedLine(float x1, float y1, float x2, float y2, float width, float space)
{
	glLineStipple(space, 0x0101);// точечный
	glLineWidth(width);			// === толщина 5
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	//glLineStipple(space, 0x1111);
}

void DrawTools::drawStrokedRect(const Rectf& rect, float lineWidth)
{
	gl::lineWidth(lineWidth);
	gl::drawLine(Vec2f(rect.x1 - lineWidth, rect.y1 - lineWidth*0.5), Vec2f(rect.x2 + lineWidth, rect.y1 - lineWidth*0.5));
	gl::drawLine(Vec2f(rect.x1 - lineWidth*0.5, rect.y1), Vec2f(rect.x1 - lineWidth*0.5, rect.y2));
	gl::drawLine(Vec2f(rect.x2 + lineWidth*0.5, rect.y1), Vec2f(rect.x2 + lineWidth*0.5, rect.y2));
	gl::drawLine(Vec2f(rect.x1 - lineWidth, rect.y2 + lineWidth*0.5), Vec2f(rect.x2 + lineWidth, rect.y2 + lineWidth*0.5));
	gl::lineWidth(1);
}
