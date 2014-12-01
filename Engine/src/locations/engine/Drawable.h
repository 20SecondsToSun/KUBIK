#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;

class Drawable
{
public:
	virtual void draw() = 0;
};