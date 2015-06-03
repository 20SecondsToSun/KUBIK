#include "CameraCanon.h"

using namespace canon;

CameraCanon::CameraCanon() :recordingFPS(12), connectionState(DISCONNECT), liveViewState(UNDEFINED)
{
	controller = new CameraController();
	controller->photoDownloadedSignal.connect(bind(&CameraCanon::photoDownloadHandler, this, placeholders::_1));
	controller->cameraShutDownSignal.connect(bind(&CameraCanon::shutdownHandler, this));
}

void CameraCanon::setup() 
{	
	controller->setDownloadedDir(getAppPath());
	connect();
}

void CameraCanon::connect() 
{
	reset();	

	try 
	{
		BaseCanon::setConnection(false);
		BaseCanon::setup(controller);

		if (BaseCanon::isCameraConnected())
		{
			connectionState = CONNECT;
			deviceConnectEvent();
		}
	}
	catch(...)
	{
		if (BaseCanon::isCameraConnected())
		{
			connectionState = DISCONNECT;
			deviceDisconnectEvent();
		}
	}	
}

void CameraCanon::setAutoReconnect(bool autoReconnect)
{
	if (!autoReconnectSignal.connected() && autoReconnect)
		autoReconnectSignal = App::get()->getSignalUpdate().connect(bind(&CameraCanon::autoReconnectCheckUpdate, this));
	else if (autoReconnectSignal.connected() && !autoReconnect)
		autoReconnectSignal.disconnect();
}

void CameraCanon::autoReconnectCheckUpdate()
{
	if(connectionState == DISCONNECT)
	{
		int seconds = (int)reconnectTimer.getSeconds();

		if (!reconnectTimer.isStopped() && seconds > 0 && seconds % 2 == 0)
		{
			reconnectTimer.stop();
			connect();
			startLiveView();
		}
		else if (reconnectTimer.isStopped())		
			reconnectTimer.start();		
	}	
}

bool CameraCanon::isConnected()
{	
	return connectionState == CONNECT;
}

void CameraCanon::reset() 
{
	mCapturedFrames.clear();
	stopRecording();	
}

void CameraCanon::startLiveView()
{
	try
	{
		BaseCanon::startLiveView();
	}
	catch(...)
	{

	}  
}

void CameraCanon::endLiveView() 
{
	try
	{
		BaseCanon::endLiveView();
	}
	catch(...)
	{

	}  
}

void CameraCanon::toggleLiveView() 
{
	isLiveViewing() ? startLiveView() : endLiveView();
}

void CameraCanon::stop()
{
	shutdown();
}

void CameraCanon::shutdown()
{
	connectionState = DISCONNECT;
	deviceDisconnectEvent();

	try
	{
		BaseCanon::shutdown();
	}
	catch (...)
	{
		
	}	
}

bool CameraCanon::checkNewFrame()
{
	bool isFrame = isFrameNew();

	if(isFrame) 	
		setFrameNew(false); 

	return isFrame;
}

void CameraCanon::takePicture()
{	
	endLiveView();

	try 
	{
		BaseCanon::takePicture();
	}
	catch(canon::ExcTakenPhoto ex)
	{
		console()<<" error message :::::  "<<ex.what()<<endl;
		photoErrorEvent();
	}
}

void CameraCanon::startRecording() 
{
	if(!_isRecording)
	{
		_isRecording = true;
		startTime = getElapsedSeconds();
	}
}

void CameraCanon::stopRecording()
{
	_isRecording = false;
}

void CameraCanon::toggleRecording() 
{
	_isRecording ? stopRecording() : startRecording();
}

void CameraCanon::pushFrame(const Surface &frame) 
{
	mCapturedFrames.push_back(frame);
}

void CameraCanon::saveFrame()
{    
	Surface liveSurface = getLiveSurface();
	int cadrHeight = liveSurface.getWidth() * 9 / 16;

	Surface frame(liveSurface.getWidth(), cadrHeight, false);
	frame.copyFrom(liveSurface, Area(0, 0, liveSurface.getWidth(), cadrHeight));
	pushFrame(frame);
} 

bool CameraCanon::isRecording() const
{
	return _isRecording;
}

int CameraCanon::getTotalCapturedFrames() const
{ 
	return mCapturedFrames.size();
}

const vector<Surface>& CameraCanon::getCapturedFrames() const
{ 
	return mCapturedFrames;
}

int CameraCanon::getWidth() const
{
	if(getLiveSurface())
	{
		return getLiveSurface().getWidth();
	}

	return 0;
}

int CameraCanon::getHeight() const
{
	Surface8u surf = getLiveSurface();

	if(surf)	
		return surf.getHeight();    

	return 0;
}

void CameraCanon::update()
{
	if (connectionState == CONNECT && isLiveViewing())	
		  downloadData();
}

void CameraCanon::draw(Rectf drawingRect) 
{ 
	color(Color::white());

	Surface liveSurface = getLiveSurface();
	if(liveSurface) 
		gl::draw(liveSurface);
}

ci::gl::Texture CameraCanon::getTexture(int sizex, int sizey, int offsetx, int offsety, float scale)
{
	auto tex = gl::Texture(getLiveSurface());
	
	if(!tex) return 0;

	gl::Fbo fbo = gl::Fbo(sizex, sizey);

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		gl::pushMatrices();		
		gl::scale(scale, scale);
		gl::translate(canon::resolution::CAMERA_HEIGHT, offsety);
		gl::rotate(90);	
		gl::draw(tex);
		gl::popMatrices();
	});

	gl::Texture retTex = fbo.getTexture();
	Utils::clearFBO(fbo);

	gl::Fbo fbo1 = gl::Fbo(sizex, sizey);
	Utils::drawGraphicsToFBO(fbo1, [&]()
	{
		gl::pushMatrices();
		gl::translate(sizex, 0);
		gl::scale(-1, 1);					
		gl::draw(retTex);
		gl::popMatrices();
	});

	gl::Texture retTex1 = fbo1.getTexture();
	Utils::clearFBO(fbo1);

	return retTex1;
}

// ----------------------------------------------------------------------------

void CameraCanon::photoCameraError( EdsError err)
{
	photoErrorEvent();
}

void CameraCanon::photoTaken(EdsDirectoryItemRef directoryItem)//, EdsError error)
{
	photoTakenEvent();
}

void CameraCanon::photoDownloadHandler(const string& downloadPath)
{
	photoDownloadedEvent(downloadPath);
}

void CameraCanon::shutdownHandler()
{
	connectionState = DISCONNECT;
	deviceDisconnectEvent();
	shutdown();
}

void CameraCanon::setDownloadDirectory(fs::path dir)
{
	fs::path downloadPath = dir;

	if(!fs::is_directory(downloadPath))
	{
		if(!fs::create_directory(downloadPath))
		{	
			downloadPath = getAppPath();
		}
	}
	controller->setDownloadedDir(downloadPath);
}

fs::path CameraCanon::photoDownloadDirectory()
{
	return controller->getDownloadedDir();
}

void CameraCanon::photoCameraReadyLiveView()
{
	try
	{
		startLiveView();		
		restartLiveViewTimer.stop();
	}
	catch(...)
	{
		restartLiveViewTimer.start();
	}
}

void CameraCanon::handleStateEvent(EdsUInt32 inEvent)
{
	switch (inEvent)
	{
	case kEdsStateEvent_Shutdown:
		endLiveView();
		connectionState = DISCONNECT;
		deviceDisconnectEvent();
		break;

	case kEdsStateEvent_WillSoonShutDown:
		extendShutDownTimer();
		break;
	}

	console() << "event--------------------------  " << inEvent<<endl;
}