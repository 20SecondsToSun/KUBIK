#include "ApplicationView.h"
#include "Controller.h"
#include "Types.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik;

class EngineApp : public AppNative 
{
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );
	void update();
	void draw();

private:	
	AppViewRef view;
	ControllerRef controller;
	AppModelRef model;
	void prepareSettings(AppBasic::Settings *settings);
};

void EngineApp::prepareSettings(AppBasic::Settings *settings)
{
	settings->setFrameRate(60);
	settings->setWindowSize(1080, 1920);// 1080, 1120);//(1880, 1120);	
	//settings->setFullScreen(true);
	//settings->setBorderless(true);	
}

void EngineApp::setup()
{	
	
	//setWindowSize(1080, 1920);
	//setFrameRate(60);
	FullScreenOptions fo;
	DisplayRef d = fo.getDisplay();
	fo.secondaryDisplayBlanking(true);
	
	
	fo.display(d->getDisplays()[0]);
	//setFullScreen(true, fo);
	// [864,1536]
	console()<<"GET WINDOWS SIZE------------------------::  "<<getWindowSize()<<endl;

	model		= AppModelRef(new ApplicationModel());
	view		= AppViewRef(new ApplicationView());
	controller  = ControllerRef(new Controller(model, view));

	gl::enableAlphaBlending();
	
}

void EngineApp::mouseDown( MouseEvent event )
{
	//setFullScreen(!isFullScreen());
	//console()<<"GET WINDOWS SIZE------------------------::  "<<getWindowSize()<<endl;
}

void EngineApp::keyDown( KeyEvent event )
{
	switch (event.getCode())
	{
		case app::KeyEvent::KEY_ESCAPE:
			quit();
		break;

		default:
		break;
	}
}

void EngineApp::update()
{

}

void EngineApp::draw()
{
	gl::clear(Color::black()); 
	view->draw();
}

CINDER_APP_NATIVE( EngineApp, RendererGl )