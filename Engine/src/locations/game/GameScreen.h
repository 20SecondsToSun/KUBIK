#pragma once
#include "cinder/app/AppNative.h"

#include "IScreen.h"
#include "Funces.h"
#include "Photobooth.h"
#include "Types.h"
#include "GameSettings.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class GameScreen: public IScreen
	{
	public:

		GameScreen(int gameId);	
		~GameScreen();

		void draw();
		void init(GameSettings* config);
		void reset();

		void closeGameHandler();
		void addMouseUpListener();
		void removeMouseUpListener();

	protected:
		void setTextures(){};

	private:
		shared_ptr<IGame> currentGame;
		int gameID;
	};
}