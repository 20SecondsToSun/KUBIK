#pragma once

#include "IScreen.h"
#include "ISettings.h"

using namespace std;
using namespace ci::signals;

namespace kubik
{
	class IGame:public IScreen
	{
	public:	
		~IGame(){};
		virtual void draw() = 0;		
		virtual void start() = 0;		
		virtual void clean(){};
		virtual void addMouseUpListener() = 0;
		virtual void removeMouseUpListener() = 0;
		signal<void(void)> closeLocationSignal;
	};
}