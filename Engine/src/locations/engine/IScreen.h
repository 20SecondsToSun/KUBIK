#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Types.h"
#include "ISettings.h"

using namespace ci::signals;
using namespace std;

namespace kubik
{
	class IScreen
	{

	//protected:
		//ResourceDictionary designTexures;

	public:	
		~IScreen()
		{
			//ci::app::console()<<"~~~~~~~~~~~~~~~~IScreen destruct~~~~~~~~~~~~~~~~~~~"<<endl;
		}
		virtual void draw() = 0;		
		virtual void init(shared_ptr<ISettings> config) = 0;
		virtual void reset(shared_ptr<ISettings> config) = 0;

		ci::signals::signal<void(void)> closeLocationSignal;
	};
}