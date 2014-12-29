#include "CameraCanon.h"

using namespace canon;

CameraCanon::CameraCanon():recordingFPS(12), connectionState(UNDEFINED), liveViewState(UNDEFINED)
{
	controller = new CameraController();
	controller->photoDownloadedSignal.connect(bind(&CameraCanon::photoDownloaded, this, placeholders::_1));
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
		BaseCanon::setup(controller);

		if (!isConnected()) 
		{
			connectionState = CONNECT;
			deviceConnectEvent();
		}
	}
	catch(...)
	{
		if (isConnected()) 
		{
			connectionState = DISCONNECT;
			deviceDisconnectEvent();
		}
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
	BaseCanon::shutdown();
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
	if (connectionState == CONNECT)
	{
		/*if(_isRecording && ((getElapsedSeconds() - startTime) >= 1.0f / recordingFPS))
		{
			saveFrame();
			startTime = getElapsedSeconds();
		}*/

		if(isLiveViewing())
		{
			downloadData();
		}
		else if (!restartLiveViewTimer.isStopped()  && (int)restartLiveViewTimer.getSeconds() == 2)
		{
			photoCameraReadyLiveView();
		}
	}
	else
	{
		int seconds = (int)reconnectTimer.getSeconds();

		if (!reconnectTimer.isStopped() && seconds > 0 && seconds % 2 == 0)
		{
			reconnectTimer.stop();
			connect();
			startLiveView();
		}
		else if (reconnectTimer.isStopped())
		{		
			reconnectTimer.start();
		}
	}
}

void CameraCanon::draw(Rectf drawingRect) 
{ 
	color(Color::white());

	Surface liveSurface = getLiveSurface();
	if(liveSurface) 
		gl::draw(liveSurface);
}

// ----------------------------------------------------------------------------

void CameraCanon::photoCameraError( EdsError err)
{
	photoErrorEvent();
}

void CameraCanon::photoTaken(EdsDirectoryItemRef directoryItem)//, EdsError error)
{
	//if (error == EDS_ERR_OK)
	//{
	//startLiveView();
	photoTakenEvent();
		//downloadImage(directoryItem);
	//}
	//else
	//	photoErrorEvent();
}

void CameraCanon::photoDownloaded(const string& downloadPath)
{
	//if (error == EDS_ERR_OK)
	//{
	//startLiveView();
	photoDownloadedEvent(downloadPath);
	//}
	//else
	//{
	//	photoErrorEvent();
	//}
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
}