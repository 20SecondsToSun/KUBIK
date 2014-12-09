#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "ISettings.h"
#include "FuncesSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class Funces:public IGame
	{
	public:	

		Funces();
		~Funces();

		void draw();	
		void init(ISettings* config);
		void reset();

		void addMouseUpListener();
		void removeMouseUpListener();

	private:	

		Texture closeImg;
		shared_ptr<Button> closeBtn;

		void mouseUp( MouseEvent &event);
		void mouseUpHandler(Button& button );

		connection mouseUpListener, closeBtnListener;	

		FuncesSettings* settings;
	};
}