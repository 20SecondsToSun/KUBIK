#pragma once
#include "ApplicationModel.h"
#include "gui/Sprite.h"
#include "IGame.h"
#include "PhotoboothSettings.h"
#include "states/PhotoInstruction.h"
#include "states/PhotoFilter.h"
#include "states/PhotoTimer.h"
#include "states/PhotoShooting.h"
#include "states/PhotoSharing.h"
#include "states/PhotoChoosing.h"
#include "states/PhotoTemplate.h"
#include "model/PhotoStorage.h"
#include "CameraAdapter.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class Photobooth:public IGame, public Sprite
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

			private:			
				void setTextures();

				PhotoInstructionRef photoInstruction;
				PhotoFilterRef		photoFilter;
				PhotoTimerRef		photoTimer;
				PhotoShootingRef	photoShooting;			
				PhotoChoosingRef	photoChoosing;
				PhotoTemplateRef	photoTemplate;
				PhotoSharingRef		photoSharing;

				std::list<IPhotoboothLocationRef>  locations;
				std::list<IPhotoboothLocationRef>::iterator currentLocation;

				void nextLocationHandler();
				void initLocations();
				void removeListeners();
				void reshotHandler();

				PhotoboothSettingsRef settings;
				PhotoStorageRef photoStorage;
			};
		}
	}
}