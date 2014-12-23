#include "BaseCanon.h"

using namespace canon;
using namespace resolution;

EdsError EDSCALLBACK DownloadImageProgress(EdsUInt32 inPercent, EdsVoid *inContext, EdsBool *outCancel)
{
	return EDS_ERR_OK;
};

BaseCanon::BaseCanon():_isCameraConnected(false), _isLiveView(false), _isFrameNew(false)
{
	
}

void BaseCanon::setup(CameraController* controller, int cameraIndex)
{
	mLivePixels = Surface8u(CAMERA_WIDTH, CAMERA_HEIGHT, false, SurfaceChannelOrder::RGB);	
	shutCon     = App::get()->getSignalShutdown().connect(bind(&BaseCanon::shutdown, this));
	init(controller);
}

void BaseCanon::init(CameraController* controller)
{	
	// Initialize sdk
	EdsError err = EdsInitializeSDK();	
	
	// Get Camera List and open a session owith one of them
	err = EdsGetCameraList(&cameraList);

	if(err != EDS_ERR_OK)
		throw ExcCouldnotGetCameraList();

	if(!getNumConnectedCameras()) 
		throw ExcCameraListEmpty();
	
	//getDeviceInfo( mCamera );
	err = EdsGetChildAtIndex(cameraList, 0, &camera);

	if(err != EDS_ERR_OK)
		throw ExcCouldnotGetCamera();	

	err = EdsOpenSession(camera);

	if(err != EDS_ERR_OK)
		throw ExcCouldnotOpenSession();	

	EdsSetObjectEventHandler(camera, kEdsObjectEvent_All,	  CameraEventListener::handleObjectEvent , (EdsVoid*)controller);
	EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All, CameraEventListener::handlePropertyEvent, (EdsVoid*)controller);	
	EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All, CameraEventListener::handleStateEvent, (EdsVoid*)controller);

	_isCameraConnected = true;	
	_isBusy = false;
	_isRunning = true;	
}

void BaseCanon::getDeviceInfo(EdsCameraRef cam)
{
	EdsDeviceInfo info;
	EdsError err = EdsGetDeviceInfo(cam, &info);

	if( err == EDS_ERR_OK )
	{
		//console() << "Cinder-Canon :: Device name :: " << info.szDeviceDescription << endl;
		//console() << "Cinder-Canon :: deviceSubType :: " << info.deviceSubType << endl;
		//console() << "Cinder-Canon :: reserved :: " << info.reserved << endl;
		//console() << "Cinder-Canon :: szPortName :: " << info.szPortName << endl;		
	}
	else
		throw ExcDeviceInfo(CanonErrorToString(err));	
}

void BaseCanon::takePicture()
{
	if(!_isCameraConnected)
		return;	

	console() << "Cinder-Canon :: Attempting to take picture "<< endl;
	
	EdsInt32 saveTarget = kEdsSaveTo_Host;
	EdsError err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, 4, &saveTarget);
	EdsCapacity newCapacity = {0x7FFFFFFF, 0x1000, 1};
	err = EdsSetCapacity(camera, newCapacity);

	err = EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);

	if(err != EDS_ERR_OK) 	
		throw ExcTakenPhoto(CanonErrorToString(err));
}

void BaseCanon::startLiveView()
{
	if(_isLiveView == false)
	{
		EdsUInt32 device;
		EdsError err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);

		if(err == EDS_ERR_OK)
		{
			device |= kEdsEvfOutputDevice_PC;
			err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);

			if (err == EDS_ERR_OK)
			{
				_isLiveView = true;
				_isBusy = false;
			}
			else
			{
				_isBusy = err == EDS_ERR_DEVICE_BUSY;
				throw ExcStartLiveView(CanonErrorToString(err));
			}
		}
		else
		{
			_isBusy = err == EDS_ERR_DEVICE_BUSY;
			throw ExcStartLiveView(CanonErrorToString(err));
		}
	}
}

void BaseCanon::endLiveView()
{	
	if(_isLiveView)
	{
		_isLiveView = false;
		// Get the output device for the live view image
		EdsUInt32 device;
		EdsError err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);

		// PC live view ends if the PC is disconnected from the live view image output device.
		if(err != EDS_ERR_OK)
			throw ExcStopLiveView(CanonErrorToString(err));
		
		device &= ~kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);

		if(err != EDS_ERR_OK)
			throw ExcStopLiveView(CanonErrorToString(err));		
	}
}

void BaseCanon::extendShutDownTimer()
{
	// always causes EDS_ERR_DEVICE_BUSY, even with live view disabled or a delay
	// but if it's not here, then the camera shuts down after 5 minutes.
	EdsError err = sendCommand(camera, kEdsCameraCommand_ExtendShutDownTimer, 0);

	if(err != EDS_ERR_OK)
		throw ExcExtendShutDownTimer(CanonErrorToString(err));
}

EdsError BaseCanon::downloadEvfData( EdsCameraRef camera )
{
	if(!_isLiveView)
	{
		console() << "No live view" << endl;
		startLiveView();
		return EDS_ERR_OK;
	}	

	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	EdsEvfImageRef evfImage = NULL;

	// Create memory stream.
	err = EdsCreateMemoryStream( 0, &stream);

	// Create EvfImageRef.
	if(err == EDS_ERR_OK)
		err = EdsCreateEvfImageRef(stream, &evfImage);	

	// Download live view image data.
	if(err == EDS_ERR_OK)
		err = EdsDownloadEvfImage(camera, evfImage);

	// Get the incidental data of the image.
	if(err == EDS_ERR_OK)
	{
		// Get the zoom ratio
		EdsUInt32 zoom;
		EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0, sizeof(zoom), &zoom);

		// Get the focus and zoom border position
		EdsPoint point;
		EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0, sizeof(point), &point);
	}

	// Display image
	EdsUInt32 length;
	unsigned char* image_data;
	EdsGetLength(stream, &length);

	if( length <= 0 )
		return EDS_ERR_OK;

	EdsGetPointer( stream, (EdsVoid**)&image_data);

	// reserve memory
	Buffer buffer(image_data, length);    
	mLivePixels = Surface(loadImage(DataSourceBuffer::create(buffer), ImageSource::Options(), "jpg"));
	_isFrameNew = true;

	// Release stream
	if(stream != NULL) 
	{
		EdsRelease(stream);
		stream = NULL;
	}

	// Release evfImage
	if(evfImage != NULL)
	{
		EdsRelease(evfImage);
		evfImage = NULL;
	}

	return EDS_ERR_OK;
}

EdsError BaseCanon::sendCommand(EdsCameraRef inCameraRef, EdsUInt32 inCommand, EdsUInt32 inParam)
{
	EdsError err = EDS_ERR_OK;

	err = EdsSendCommand( inCameraRef, inCommand, inParam );

	if(err != EDS_ERR_OK)
	{
		console() << "Cinder-Canon :: error while sending command " <<  CanonErrorToString(err) << "." << endl;
		if(err == EDS_ERR_DEVICE_BUSY) {
			//return false;
		}
	}

	return err;
}

void BaseCanon::downloadData()
{	
	downloadEvfData(camera); 
}

void BaseCanon::setImageQuality(EdsImageQuality quality)
{
	////////////////////////////////////////
	//
	// EdsImageQuality_LR  -4290x2856
	// EdsImageQuality_LRLJF  - 4272x2848
	//EdsImageQuality_LJF - 4272x2848
	//EdsImageQuality_LJN
	//EdsImageQuality_MJF
	//EdsImageQuality_S1JF 2256-1504
	//EdsImageQuality_S2JF 1920-1080
	//EdsImageQuality_S3JF 720-480

	quality = EdsImageQuality_S2JF;
	EdsError err = EdsSetPropertyData(camera, kEdsPropID_ImageQuality, 0, sizeof(quality), &quality);
	console()<<"image quality set: "<<quality<<"  "<<CanonErrorToString(err)<<"    "<<endl;
}

void BaseCanon::setWhiteBalance(EdsWhiteBalance m_whiteBalance)
{	
	EdsError err = EdsSetPropertyData(camera, kEdsPropID_WhiteBalance, 0, sizeof(m_whiteBalance), (EdsVoid *)&m_whiteBalance);
	console()<<"m_whiteBalance set: "<<m_whiteBalance<<"  "<<CanonErrorToString(err)<<"    "<<endl;
}

void BaseCanon::setFrameNew(bool value)
{ 
	_isFrameNew = value; 
}

int BaseCanon::getNumConnectedCameras() const
{
	EdsUInt32 numCameras = 0;
	EdsError err = EdsGetChildCount( cameraList, &numCameras);

	if( err != EDS_ERR_OK )
		numCameras = 0;

	return numCameras;
}

Surface8u BaseCanon::getLiveSurface() const
{
	return mLivePixels;
}

bool BaseCanon::isCameraConnected() const
{
	return _isCameraConnected;
};

bool BaseCanon::isLiveViewing() const
{ 
	return _isLiveView;
};

bool BaseCanon::isBusy() const
{
	return _isBusy;
};

bool BaseCanon::isFrameNew() const
{
	return _isFrameNew;
}

void BaseCanon::shutdown()
{ 
	console()<<"SHUTDOWN!!!!!!"<<endl;
	shutCon.disconnect();
	_isRunning = false;
	endLiveView();

	EdsError err = EdsCloseSession(camera);

	if(err == EDS_ERR_OK)	
		EdsTerminateSDK();
}