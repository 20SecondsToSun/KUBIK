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
	settings->setWindowSize(1080, 1120);//(1880, 1070);	
	//settings->setBorderless(true);	
}

void EngineApp::setup()
{	
	model		= AppModelRef(new ApplicationModel());
	view		= AppViewRef(new ApplicationView());
	controller  = ControllerRef(new Controller(model, view));

	gl::enableAlphaBlending();
}

void EngineApp::mouseDown( MouseEvent event )
{

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