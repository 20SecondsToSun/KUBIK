#pragma once
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IScreen.h"
#include "TuneUpSettings.h"
#include "MenuSettings.h"
#include "Button.h"
#include "GameSettings.h"
#include "ScreenSaverSettings.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace params;

namespace kubik
{
	class TuneUpScreen:public IScreen
	{
	public:

		TuneUpScreen(shared_ptr<TuneUpSettings> screenSaverSettings,
					 shared_ptr<ScreenSaverSettings> config,
					 shared_ptr<MenuSettings>        menuConfig,
					 shared_ptr<GameSettings>		 gameSettings);

		~TuneUpScreen();

		void draw();	
		void init(shared_ptr<ISettings> settings) override;
		void reset(shared_ptr<ISettings> config) override{};
		void addMouseUpListener();
		void removeMouseUpListener();
		void startUpParams();
		void savePhtbtn();
		
		signal<void(void)> closeSettingsSignal;
		signal<void(vector<Changes>)> appSettingsChangedSignal;

		connection mouseUpListener;
		connection closeBtnListener;
		connection appSettingsChgListener;

	private:

		InterfaceGlRef	photoBoothParams;
		InterfaceGlRef	menuParams;
		InterfaceGlRef	gamesParams;
		InterfaceGlRef	funcesParams;

		vector<InterfaceGlRef> params;

		shared_ptr<TuneUpSettings>		tuneUpSettings;
		shared_ptr<ScreenSaverSettings> screnSaversettings;
		shared_ptr<MenuSettings>		menuSettings;
		shared_ptr<GameSettings>		gameSettings;

		shared_ptr<ButtonText>	saveChngBtn;
		shared_ptr<Button>		closeBtn;
		Font font;

		vector<Changes> changes;

		PhotoboothSettings::PhotoboothDataStruct phData, initPhData;
		MenuSettings::MenuDataStruct menuData, initialMenuData;		
		GameSettings::GamesDataStruct gamesData, initialGamesData;	

		void update();
		void mouseUp(MouseEvent &event);	
		void closeLocationHandler(Button& button);
		void appSettingsChgHandler(ButtonText& button);		
		
		void savePhotoboothParams();

		void createPhotoboothParams();
		void createMenuParams();	
		void createFuncesParams();	

		void checkPhotoBoothParamsForChanges();
		void checkFuncesParamsForChanges();
		void checkMenuParamsForChanges();
		void checkGamesParamsForChanges();
		
		void createGamesParams();

		void setDefaultGameIdInSwitchOnGames();
		void setReloadGamePropertyIfNeedIt(Changes &chng);
	};
}