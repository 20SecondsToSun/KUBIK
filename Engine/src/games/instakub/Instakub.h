#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "InstakubSettings.h"
#include "gui/Sprite.h"
#include "instakub/states/InstakubLocation.h"
#include "instakub/states/HashtagOnly.h"
#include "instakub/states/SearchOnly.h"
#include "instakub/states/HashtagAndSearch.h"

#include "TimerTools.h"

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			class Instakub :public IGame
			{
			public:
				Instakub(ISettingsRef setRef);
				~Instakub();				
				
				void start();
				void draw();
				void update(){};

				void stop() override;
				void reset() override;
				void create();				
				void init(ISettingsRef config) override;
				void showAnimationComplete() override;

			private:
				static const float RELOAD_TIME;

				ci::gl::Texture closeImg;
			
				InstakubSettingsRef settings;
				InstakubLocationRef view;
				HashtagOnlyRef hashtagOnly;
				SearchOnlyRef searchOnly;
				HashtagAndSearchRef hashTagAndSearch;	

				void closeMouseUpHandler(IButton& button);
				void mouseTouched(EventGUIRef& event);

				void enableControls();
				void disableControls();
				void showControls();
				void hideControls();

				void stopTimer();
				void startTimer();
				void resetTimer();
				void timerComplete();

				void viewDrawings();
				void animDrawings();

				void (Instakub::* drawFuncPointer)();
				void changeState(const locationStates& state);
			};		
		}
	}
}