#pragma once
#include "instakub/states/SearchLocation.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class SearchButtons> SearchButtonsRef;

			class SearchButtons : public kubik::Sprite
			{
				ImageButtonSpriteRef hashTagButton, userButton;
				ci::Anim<float> alpha;
				ci::gl::Texture hashtagTex, userOverTex, hashtagOverTex, userTex;

				void hashTagTouched(EventGUIRef& event);
				void userTouched(EventGUIRef& event);			

			public:
				enum searchMode
				{
					HASHTAG,
					USER
				}
				mode;

				SearchButtons(const ci::gl::Texture& tex1, 
					const ci::gl::Texture& tex2,
					const ci::gl::Texture& tex3, 
					const ci::gl::Texture& tex4);

				void hide();
				void show();
				void draw() override;

				void connect();
				void disconnect();
				
				void setDefaultSearchMode();
				searchMode getActiveSearchMode() const;
				bool hashtagSearchMode() const;
			};
		}
	}
}