#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Curl.h"
#include "InstagramThreadClient.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik::instagram;

class InstagramApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	void loadingDataComplete();

	InstagramClientRef instClient;
};

void InstagramApp::setup()
{
	setWindowSize(1224, 1000);
	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient = InstagramClientRef(new InstagramClient(clientID));
	//instClient->LoadingCompleteSignal.connect(bind(&InstagramApp::loadingDataComplete, this));
	//instClient->userSearch("metalcorehero");
	//instClient->loadUserFeed(instClient->getUsers().front().getID());
	
	instClient->loadTagMedia("nofilter");
	
	//instClient->loadPopularMedias();
}

void InstagramApp::mouseDown( MouseEvent event )
{
	//std::map<string,string> strings;
	//strings.insert(pair<string, string>( "client_id" , "6ac8af15a5d341e9802c8d1a26899ae3"));
	//strings.insert(pair<string, string>( "redirect_uri" , "http://familyagency.ru/"));
	//strings.insert(pair<string, string>( "response_type" , "code"));

	string instUrl = "https://api.instagram.com/oauth/authorize/";	
	//https://api.instagram.com/oauth/authorize/?client_id=6ac8af15a5d341e9802c8d1a26899ae3&redirect_uri=http://familyagency.ru/&response_type=code
	//instUrl = "https://instagram.com/oauth/authorize/?client_id=6ac8af15a5d341e9802c8d1a26899ae3&redirect_uri=http://familyagency.ru/&response_type=token";

	
	//InstagramViewerRef  viewer = InstSimpleViewerRef(new InstSimpleViewer());
	//viewer->provideMediaList();

}

void InstagramApp::loadingDataComplete()
{
	console()<<"LOADED!!!!!!!!!!!!!"<<endl;
}

void InstagramApp::update()
{
}

void InstagramApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	int x = 0, y = 0, i = 0;
	for (auto tex : instClient->getImages())
	{
		x = tex.getWidth()  * (i % 4);
		y = tex.getHeight() * (i / 4);

		gl::draw(tex, Vec2f(x,y));

		i++;
	}
	
	//viewer->draw();
}

CINDER_APP_NATIVE( InstagramApp, RendererGl )
