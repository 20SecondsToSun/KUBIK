#pragma once

#include "ApplicationModel.h"
#include "gui/Sprite.h"
#include "IGame.h"
#include "PhotoboothSettings.h"
#include "states/instruction/PhotoInstruction.h"
#include "states/filter/PhotoFilter.h"
#include "states/timer/PhotoTimer.h"
#include "states/shooting/PhotoShooting.h"
#include "states/sharing/PhotoSharing.h"
#include "states/choosing/PhotoChoosing.h"
#include "states/template/PhotoTemplate.h"
#include "model/PhotoStorage.h"
#include "CameraAdapter.h"
#include "ScreenStorage.h"
#include "Chromium.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class Photobooth :public IGame 
			{
			public:				
				Photobooth(config::ISettingsRef config);
				~Photobooth();

				void start()	override;
				void stop()		override;
				void update()	override;
				void draw()		override;
				void reset()	override;
				void init(config::ISettingsRef config) override;
				void create();

				ci::signals::connection updateSignal;
				size_t index;					

			private:
				PhotoInstructionRef photoInstruction;
				PhotoFilterRef		photoFilter;
				PhotoTimerRef		photoTimer;
				PhotoShootingRef	photoShooting;
				PhotoChoosingRef	photoChoosing;
				PhotoTemplateRef	photoTemplate;
				PhotoSharingRef		photoSharing;

				std::vector<IPhotoboothLocationRef>  locations;
				IPhotoboothLocationRef	currentLocation;

				config::PhotoboothSettingsRef settings;
				PhotoStorageRef photoStorage;

				virtual void showAnimationComplete() override;

				void nextLocationHandler();
				void initLocations();
				void removeListeners();
				void reshotHandler();
				void cameraSetup();
				void gotoFirstlocation();
				void beginAnimHandler();
				void completeAnimHandler();
				void enableGameCloseHandler();
				void closeLocationHandler();				
				void disableGameCloseHandler();
				void handleCameraConnection();
				void drawCameraErrorPopup();
				void goToPhotoInstructionTimeOut();				

				template <class T>
				bool equalLocations(IPhotoboothLocationRef location)
				{
					IPhotoboothLocation *ev = location.get();
					return (typeid(*ev) == typeid(T));
				}
			};
		}
	}
}
