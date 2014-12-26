#pragma once

#include "ISettings.h"

using namespace std;
using namespace ci::signals;

namespace kubik
{
	class ILocation
	{

	public:	
		~ILocation(){};

		virtual void draw() = 0;
		virtual void init(shared_ptr<ISettings>) = 0;
		virtual void reset(shared_ptr<ISettings>) = 0;
		virtual void mouseUpHandler(ci::Vec2i vec) = 0;
		signal<void(void)> nextLocationSignal;		
	};
}