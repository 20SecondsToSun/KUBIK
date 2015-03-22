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

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class Photobooth :public IGame 
			{
			public:
				static const int ENABLE_GAME_CLOSE = 1;
				static const int DISABLE_GAME_CLOSE = 2;
				Photobooth(ISettingsRef config);
				~Photobooth();

				void start() override;
				void stop() override;
				void update() override;
				void draw() override;
				void reset() override;
				void init(ISettingsRef config) override;
				void create();

				connection updateSignal;
				size_t index;

				enum locationStates
				{
					SHOW_ANIM,
					DRAW
				} 
				state;
				ci::Anim<float> animX, animX1, alpha;

			private:
				ci::gl::Texture		screenshot;

				PhotoInstructionRef photoInstruction;
				PhotoFilterRef		photoFilter;
				PhotoTimerRef		photoTimer;
				PhotoShootingRef	photoShooting;
				PhotoChoosingRef	photoChoosing;
				PhotoTemplateRef	photoTemplate;
				PhotoSharingRef		photoSharing;

				std::vector<IPhotoboothLocationRef>  locations;
				IPhotoboothLocationRef	currentLocation;

				PhotoboothSettingsRef settings;
				PhotoStorageRef photoStorage;

				void nextLocationHandler();
				void initLocations();
				void removeListeners();
				void reshotHandler();
				void cameraSetup();
				void gotoFirstlocation();
				void initShowAnimation();
				void showAnimationComplete();
				void beginAnimHandler();
				void completeAnimHandler();
			};
		}
	}
}