#pragma once
#include "instakub/states/InstakubLocation.h"
#include "instakub/states/SearchButtons.h"
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
				virtual void loadingCompleteHandler();
				virtual void startLoadHandler();
				virtual	void noMoreLoadsHandler();				

				void resetInputField();					
				void connectKeyboard();

				void hashTagPhotosLoad();
				void userPhotosLoad();				

			protected:	
				static const ci::Vec2f SHOWING_KEYBORAD_POSITION;
				static const float SHOWING_KEYBORAD_TIME;
				static const float HIDING_KEYBORAD_TIME;
				static const ci::Vec2f HIDING_KEYBORAD_POSITION;

				std::shared_ptr<class SearchButtons> searchBtns;

				ci::gl::Texture searchField, searchFieldRed;
				ci::Anim<float> alphaKeyBoardBg, alphaError;
				KeyBackgroundRef keyBackground;
				string searchingText;		

				virtual void closeKeyboardHandler();
				virtual void inputTouchHandler();
				virtual void searchTouchHandler();
				virtual void openPopupHandler();
				virtual	void closePopupHandler();

				void initVirtualKeyboard();
				void disconnectKeyboard();
				void disconnectKeyboardListeners();				
				void drawTouchKeyboardLayout();
				void hideKeyboardLayout();

				void draw() override;
				void reload() override;
				void loadStrategity() override;

			public:
				SearchLocation(InstakubSettingsRef settings, const Vec2f& position);
				void stop() override;
				void load() override;
				void start() override;
				void reset() override;
			};
		}
	}
}