#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "states/PhotoInstruction.h"
#include "states/PhotoFilter.h"
#include "states/PhotoTimer.h"
#include "PhotoboothSettings.h"
#include "states/PhotoShooting.h"
#include "states/PhotoSharing.h"

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
		void start();	
		void reset(shared_ptr<ISettings> config) override;
		void create();
		void init(shared_ptr<ISettings> config) override;
		
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
		shared_ptr<PhotoShooting>	photoShooting;
		shared_ptr<PhotoSharing>	photoSharing;
		
		list<shared_ptr<IPhotoboothLocation>>  locations;
		list<shared_ptr<IPhotoboothLocation>>::iterator currentLocation;

		void nextLocationHandler();
		void initLocations();

		shared_ptr<PhotoboothSettings> settings;
	};
}