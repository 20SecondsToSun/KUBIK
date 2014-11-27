#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timer.h"
#include "Location.h"

using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace gl;
using namespace std;

class ConfigurationGameScreen : public Location
{
	public:

		void setup();
		void init( LocationEngine* game);
		void update();
		void draw();
		void cleanup(){};
	
		void handleEvents(){};
		void mouseEvents(int type){};
		void keyEvents(){};		

		static ConfigurationGameScreen* Instance()
		{
			return &ConfigurationGameScreenState;
		}

	private:

		LocationEngine* _game;
		static ConfigurationGameScreen ConfigurationGameScreenState;
	
};