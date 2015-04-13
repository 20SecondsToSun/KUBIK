#pragma once
#include "instakub/states/InstakubLocation.h"
#include "TextTools.h"

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class HashtagOnly> HashtagOnlyRef;

			class HashtagOnly : public InstakubLocation
			{	
				ci::gl::Texture hashtagPlashka, hashtagPlashkaText, hashTagTexture;
				ci::Vec2f hashtagPlashkaPos, hashTagTexturePos, hashtagPlashkaTextPos;
			public:
				HashtagOnly(InstakubSettingsRef settings);
				void load() override;
				void start() override;
				void reset() override;
				void draw() override;
				void stop() override;
			};
		}
	}
}