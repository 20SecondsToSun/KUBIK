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
			public:
				HashtagOnly(InstakubSettingsRef settings);
				virtual void start();
				virtual void reset();
				virtual void draw();
				virtual void stop(){};
			};
		}
	}
}