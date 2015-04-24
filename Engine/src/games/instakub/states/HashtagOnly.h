#pragma once
#include "instakub/states/InstakubLocation.h"
#include "TextTools.h"
#include "TimerTools.h"

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class HashtagOnly> HashtagOnlyRef;

			class HashtagOnly : public InstakubLocation
			{
				static const float TIME_TO_BACK_PLASHKA;

				ci::gl::Texture hashtagPlashka, hashtagPlashkaText, hashTagTexture;
				ci::Vec2f hashtagPlashkaPos, hashTagTexturePos, hashtagPlashkaTextPos;
				ci::Anim<ci::Vec2f> hashtagAnimPosition;

				void touchDownHandler();					
				void initViewerHandlers() override;			

			public:
				HashtagOnly(InstakubSettingsRef settings);
				void load() override;
				void start() override;
				void reset() override;
				void draw() override;
				void stop() override;
				void timeOutReload() override;				
			};
		}
	}
}