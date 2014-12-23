#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "PhotoboothSettings.h"
#include "states/PhotoInstruction.h"
#include "states/PhotoFilter.h"
#include "states/PhotoTimer.h"
#include "states/PhotoShooting.h"
#include "states/PhotoSharing.h"
#include "states/PhotoChoosing.h"
#include "states/PhotoTemplate.h"
#include "states/PhotoProcessing.h"
#include "model/PhotoStorage.h"
#include "CameraAdapter.h"

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

		void update();	
		void draw();	
		void start();
		void create();
		void reset(shared_ptr<ISettings> config) override;		
		void init(shared_ptr<ISettings> config) override;

		void addMouseUpListener();
		void removeMouseUpListener();

		connection updateSignal;

	private:		
		shared_ptr<Button> closeBtn;

		void setTextures();
		void mouseUp(MouseEvent& event);
		void mouseUpHandler(Button& button);

		connection mouseUpListener, closeBtnListener;	

		shared_ptr<PhotoInstruction>photoInstruction;
		shared_ptr<PhotoFilter>		photoFilter;
		shared_ptr<PhotoTimer>		photoTimer;
		shared_ptr<PhotoShooting>	photoShooting;
		shared_ptr<PhotoProcessing>	photoProcessing;		
		shared_ptr<PhotoChoosing>	photoChoosing;
		shared_ptr<PhotoTemplate>	photoTemplate;
		shared_ptr<PhotoSharing>	photoSharing;

		list<shared_ptr<IPhotoboothLocation>>  locations;
		list<shared_ptr<IPhotoboothLocation>>::iterator currentLocation;

		void nextLocationHandler();
		void initLocations();

		shared_ptr<PhotoboothSettings> settings;
		shared_ptr<PhotoStorage> photoStorage;
	};
}