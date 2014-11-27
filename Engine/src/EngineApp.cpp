#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Config.h"
#include "Graphics.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class EngineApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	void configLoadingComplete();
	void graphicsLoadingComplete();
	void errorHandler();
	
private:
	ApplicationModel model;
	void prepareSettings( ci::app::AppBasic::Settings *settings );
};

void EngineApp::prepareSettings( ci::app::AppBasic::Settings *settings )
{
	settings->setWindowSize(1200, 600);	
	settings->setBorderless(true);
}

void EngineApp::setup()
{	
	config().addCompleteListener(bind(&EngineApp::configLoadingComplete, this ));
	config().addErrorListener(bind(&EngineApp::errorHandler, this ));
	config().load(&model);
}

void EngineApp::configLoadingComplete()
{
	graphics().addCompleteListener(bind(&EngineApp::graphicsLoadingComplete, this ));
	graphics().addErrorListener(bind(&EngineApp::errorHandler, this ));
	graphics().loadTextures(Graphics::gamesTexturesID::PERMANENT_IN_MEMORY_TEXTURES);
}

void EngineApp::graphicsLoadingComplete()
{

}

void EngineApp::errorHandler()
{

}

void EngineApp::mouseDown( MouseEvent event )
{
	
}

void EngineApp::update()
{
}

void EngineApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( EngineApp, RendererGl )