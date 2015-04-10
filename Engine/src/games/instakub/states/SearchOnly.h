#pragma once
#include "instakub/states/InstakubLocation.h"
#include "KeyBackground.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class SearchOnly> SearchOnlyRef;

			class SearchOnly : public InstakubLocation
			{
				static const ci::Vec2f HIDING_KEYBORAD_POSITION;
				static const ci::Vec2f SHOWING_KEYBORAD_POSITION;
				static const float HIDING_KEYBORAD_TIME;
				static const float SHOWING_KEYBORAD_TIME;

				ci::gl::Texture searchField, searchFieldRed;

				void drawTouchKeyboardLayout();

				ci::Anim<float> alphaKeyBoardBg, alphaError;

				KeyBackgroundRef keyBackground;
				void initVirtualKeyboard();
				void inputTouchHandler();
				void searchTouchHandler();				
				void closeKeyboardHandler();
				void disconnectKeyboard();
				void connectKeyboard();


				virtual void openPopupHandler();

				virtual void loadingCompleteHandler();
				virtual void startLoadHandler();
				virtual	void noMoreLoadsHandler();
				virtual	void closePopupHandler();			

			public:
				SearchOnly(InstakubSettingsRef settings);

				virtual void stop();
				virtual void start();
				virtual void load();
				virtual void draw();
				virtual void reset();
				virtual void reload();
			};
		}
	}
}