#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Graphics.h"
#include "ApplicationView.h"
#include "Controller.h"
//#include <vld.h>

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
	shared_ptr<ApplicationView> view;
	shared_ptr<Controller> controller;
	void prepareSettings(ci::app::AppBasic::Settings *settings);
};

void EngineApp::prepareSettings( ci::app::AppBasic::Settings *settings)
{
	settings->setWindowSize(1200, 600);	
	//settings->setBorderless(true);	
}

void EngineApp::setup()
{	
	view		= shared_ptr<ApplicationView>(new ApplicationView());
	controller  = shared_ptr<Controller>(new Controller(view));
	controller->initLoad();

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