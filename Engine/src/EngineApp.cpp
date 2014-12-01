#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Config.h"
#include "Graphics.h"
#include "Application.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class EngineApp : public AppNative 
{
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );
	void update();
	void draw();

	void configLoadingComplete();
	void graphicsLoadingComplete();
	void errorHandler();

private:
	ApplicationModel* model;
	Application* app;
	void prepareSettings(ci::app::AppBasic::Settings *settings);
};

void EngineApp::prepareSettings( ci::app::AppBasic::Settings *settings)
{
	settings->setWindowSize(1200, 600);	
	//settings->setBorderless(true);	
}

void EngineApp::setup()
{
	model = new ApplicationModel();

	config().addCompleteListener(bind(&EngineApp::configLoadingComplete, this));
	config().addErrorListener(bind(&EngineApp::errorHandler, this));
	config().load(model);
}

void EngineApp::configLoadingComplete()
{
	graphics().addCompleteListener(bind(&EngineApp::graphicsLoadingComplete, this));
	graphics().addErrorListener(bind(&EngineApp::errorHandler, this));	

	graphics().loadTextures(Graphics::gamesTexturesID::MENU);	
	graphics().loadTextures(Graphics::gamesTexturesID::SETTINGS);		
	graphics().loadTextures(model->getDefaultGameID());	
}

void EngineApp::graphicsLoadingComplete()
{
	if(graphics().checkAllLoading())
	{
		console()<<"Graphics all Loaded:: !! "<<graphics().checkAllLoading()<<endl;

		// controller.addUnloadGameListener()
		// controller.addLoadGameListener()

		setWindowSize(1400, 800);	

		app = new Application();
		app->init(model, graphics().getTextures());
	}
}

void EngineApp::errorHandler()
{

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
	gl::clear( Color::black()); 
	if(graphics().checkAllLoading())
		app->draw();
}

CINDER_APP_NATIVE( EngineApp, RendererGl )