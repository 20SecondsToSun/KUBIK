#pragma once

#include "ApplicationModel.h"
#include "MenuButton.h"

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
#include "IDispatcher.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class Photobooth:public IGame, public IDispatcher
	{
	public:	
		Photobooth(ISettingsRef config);
		~Photobooth();

		void start();
		void stop() override;

		void update();	
		void draw();	
		void create();
		void reset() override;		
		void init(ISettingsRef config) override;	
		connection updateSignal;
		virtual void mouseUp(MouseEvent &event) override;		

	private:		
		ButtonRef closeBtn;
		void setTextures();
		void mouseUpHandler(IButton& button);
		connection mouseUpListener, closeBtnListener;	

		PhotoInstructionRef photoInstruction;
		PhotoFilterRef		photoFilter;
		PhotoTimerRef		photoTimer;
		PhotoShootingRef	photoShooting;
		PhotoProcessingRef	photoProcessing;		
		PhotoChoosingRef	photoChoosing;
		PhotoTemplateRef	photoTemplate;
		PhotoSharingRef		photoSharing;

		list<IPhotoboothLocationRef>  locations;
		list<IPhotoboothLocationRef>::iterator currentLocation;

		void nextLocationHandler();
		void initLocations();

		PhotoboothSettingsRef settings;
		PhotoStorageRef photoStorage;
	};
}