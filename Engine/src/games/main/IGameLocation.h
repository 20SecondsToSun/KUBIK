#pragma once

#include "PozaSettings.h"
#include "main/PhotoStorage.h"
#include "gui/Sprite.h"
#include "main/DataBaseRecord.h"

namespace kubik
{
	namespace games
	{
		typedef std::shared_ptr<class IGameLocation> IGameLocationRef;

		class IGameLocation : public Sprite
			{
			protected:
				static config::ISettingsRef settings;
				ci::gl::TextureRef bckgrnd;
				float titlePositionY, animShowTitleTime;

				ci::Anim<float> titleAlpha, titleScale, titleFilterAlpha;
				ci::Anim<ci::Vec2f> titleAnimPosition;
				ci::gl::Texture title, screenshot;
				std::shared_ptr<DataBaseRecord> dbRecord;

				void setLastScreenShot();

			public:
				static enum Command
				{
					NEXT_LOC,
					BEGIN_ANIM,
					COMPLETE_ANIM,
					ENABLE_GAME_CLOSE,
					DISABLE_GAME_CLOSE,
					CLOSE_LOCATION,
					FIRST_LOC,
				};				

				IGameLocation();

				virtual void update() = 0;
				virtual void start() = 0;
				virtual void stop(){};
				virtual void reset(config::ISettingsRef settings);
				virtual void stopAllTweens();

				void setDbRecord(std::shared_ptr<DataBaseRecord> db);

				void nextLocationSignal(EventGUIRef& event = EventGUIRef(new EventGUI()));
				void hideAnimationComplete();
				void fillBg();
				void drawTitle();

				template <class T>
				bool equalLocations(IGameLocationRef location)
				{
					auto *ev = location.get();
					return (typeid(*ev) == typeid(T));
				}
			};
	}	
}
