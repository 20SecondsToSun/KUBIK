#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "PhotoInstruction.h"
#include "PhotoFilter.h"
#include "PhotoTimer.h"
#include "PhotoboothSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;
namespace kubik
{
	class Photobooth:public IGame
	{
	public:	

		Photobooth(shared_ptr<ISettings> config);
		~Photobooth();

		void draw();	
		void reset();
		void create();
		void init(shared_ptr<ISettings> config);
		
		void addMouseUpListener();
		void removeMouseUpListener();

	private:	

		Texture closeImg;
		shared_ptr<Button> closeBtn;

		void setTextures();
		void mouseUp( MouseEvent &event);
		void mouseUpHandler(Button& button );

		connection mouseUpListener, closeBtnListener;	

		shared_ptr<PhotoInstruction> photoInstruction;
		shared_ptr<PhotoFilter>	photoFilter;
		shared_ptr<PhotoTimer>	photoTimer;

		list<shared_ptr<ILocation>>  locations;
		list<shared_ptr<ILocation>>::iterator currentLocation;

		void nextLocationHandler();

		shared_ptr<PhotoboothSettings> settings;
	};
}