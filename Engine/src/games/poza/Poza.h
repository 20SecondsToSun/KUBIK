#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "PozaSettings.h"
#include "gui/Sprite.h"
#include "states/PozaInstruction.h"
#include "states/ThreeMeters.h"
#include "states/HandsUp.h"
#include "states/Timer.h"
#include "states/PozaGame.h"
#include "states/Printer.h"
#include "main/SocialLocation.h"
#include "model/PozaBase.h"
#include "kinect2/KinectAdapter.h"
#include "StatCollector.h"
#include "model/HumanModel.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			class Poza :public IGame
			{
			public:
				Poza(config::ISettingsRef setRef);
				~Poza();

				static const int kinectShiftY = -660;
				static float humanHeight;
				

				virtual void draw() override;
				virtual void update() override;
				void create();
				void cameraSetup();
				void kinectSetup();

				virtual void start() override;
				virtual void stop() override;
				virtual void reset() override;
				virtual void init(config::ISettingsRef config) override;
				virtual void showAnimationComplete() override;				

			private:
				PozaBase pozaData;
				ci::gl::Texture closeImg;
				ci::signals::connection mouseUpListener, closeBtnListener;
				config::PozaSettingsRef settings;
				config::StatCollectorRef statSettings;
				
				std::vector<IGameLocationRef>  locations;
				IGameLocationRef currentLocation;

				PozaInstructionRef pozaInstruction;
				ThreeMetersRef triMeters;
				HandsUpRef handsUp;
				PozaTimer1Ref timer;
				PozaGameRef game;
				SocialLocationRef social;
				PrinterRef printer;

				photobooth::PhotoStorageRef photoStorage;
				std::shared_ptr<DataBaseRecord> dbRecord;
				std::shared_ptr<HumanModel> humanModel;

				std::vector<int> gamesScore;
				int index;
				ci::signals::connection updateSignal;

				void closeMouseUpHandler(IButton& button);
				void initLocations();
				void removeListeners();
				void nextLocationHandler();
				void firstLocationHandler();				
				void gotoFirstlocation();
				void closeLocationHandler();

				void beginAnimHandler();
				void completeAnimHandler();
				void enableGameCloseHandler();
				void disableGameCloseHandler();

				void handleCameraConnection();
				void drawCameraErrorPopup();
				void goToPhotoInstructionTimeOut();	

				void saveDbRecord();
			};
		}
	}
}