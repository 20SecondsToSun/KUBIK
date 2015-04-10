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

			class SearchLocation : public InstakubLocation
			{				
				static const ci::Vec2f SHOWING_KEYBORAD_POSITION;				
				static const float SHOWING_KEYBORAD_TIME;

				virtual void openPopupHandler();
				virtual void loadingCompleteHandler();
				virtual void startLoadHandler();
				virtual	void noMoreLoadsHandler();
				
				virtual void closeKeyboardHandler();
				
				void inputTouchHandler();
				void searchTouchHandler();
				
				void connectKeyboard();
					

			protected:	
				static const float HIDING_KEYBORAD_TIME;
				static const ci::Vec2f HIDING_KEYBORAD_POSITION;

				ci::gl::Texture searchField, searchFieldRed;
				ci::Anim<float> alphaKeyBoardBg, alphaError;
				KeyBackgroundRef keyBackground;

				virtual	void closePopupHandler();
				virtual void loadStrategity() = 0;
				void initVirtualKeyboard();
				void disconnectKeyboard();
				void drawTouchKeyboardLayout();

			public:
				SearchLocation(InstakubSettingsRef settings, const Vec2f& position);
				virtual void stop() override;
				virtual void load() override;
				virtual void start() override;
			};
		}
	}
}