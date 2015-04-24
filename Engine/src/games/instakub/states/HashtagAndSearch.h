#pragma once
#include "instakub/states/SearchLocation.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class HashtagAndSearch> HashtagAndSearchRef;

			class HashtagAndSearch : public SearchLocation
			{				
				void closeKeyboardHandler() override;
				void inputTouchHandler() override;	
				void openPopupHandler() override;
				void closePopupHandler() override;
				void showDefaultHashtag();
				
				ci::Anim<float> hashTagAlpha;
				ci::gl::Texture hashtagTexture;
				ci::Vec2f hashtagPosition;

			public:	
				HashtagAndSearch(InstakubSettingsRef settings);
				void load() override;
				void draw() override;
				void start() override;
				void reset() override;
				void timeOutReload() override;
			};
		}
	}
}