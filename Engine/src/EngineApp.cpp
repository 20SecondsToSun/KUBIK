#include "ApplicationView.h"
#include "Controller.h"
#include "Types.h"
#include "kinect2/KinectAdapter.h"
#include "server/Server.h"
#include "printer/Printer.h"

#define release

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik;
using namespace kubik::config;

class EngineApp : public AppNative 
{
public:
	void setup();
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void keyDown( KeyEvent event );
	void update();
	void draw();
	void shutdown();

private:	
	AppViewRef view;
	ControllerRef controller;
	ApplicationModelRef model;
	void prepareSettings(AppBasic::Settings* settings);

	ci::Vec2f touchPoint;
};

void EngineApp::prepareSettings(AppBasic::Settings* settings)
{
	settings->setFrameRate(60);
	settings->setWindowSize(1080, 1920);
	settings->setWindowPos(100, 100);
}

void EngineApp::setup()
{
	gl::enableAlphaBlending();


	server().init();
	server().login();
	
	printer().GetPrinterList();

	// [864,1536]  problem with resolution https://forum.libcinder.org/#Topic/23286000002138001

	model = ApplicationModelRef(new ApplicationModel());
	view = AppViewRef(new ApplicationView());
	controller = ControllerRef(new Controller(model, view));

	FullScreenOptions fullscreenOptions;
	DisplayRef display = fullscreenOptions.getDisplay();

	if (display->getDisplays().size() > 1)
	{
	//	fullscreenOptions.display(display->getDisplays()[1]);
	}

#ifdef release
	////fullscreenOptions.secondaryDisplayBlanking(true);	
	setFullScreen(true, fullscreenOptions);
#endif
}

void EngineApp::mouseDown(MouseEvent event)
{
	//event.mHandled = true;
}

void EngineApp::mouseUp(MouseEvent event)
{
	//event.mHandled = true;

	touchPoint = Utils::transformCoords(event.getPos());
	touchPoint = event.getPos(); //Utils::transformCoords(event.getPos());
}

void EngineApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
		case app::KeyEvent::KEY_ESCAPE:
			quit();
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

	//gl::drawSolidCircle(touchPoint, 20, 20);
}

void EngineApp::shutdown()
{
	// kill singletones
	kinect().stop();
}

CINDER_APP_NATIVE( EngineApp, RendererGl )