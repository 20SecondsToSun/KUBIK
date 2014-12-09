#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Types.h"

using namespace ci::signals;
using namespace std;

namespace kubik
{
	class IScreen
	{

	protected:
		Types::OneBlockTexDictionary designTexures;

	public:	
		virtual void draw() = 0;
		ci::signals::signal<void(void)> closeLocationSignal;
	};
}