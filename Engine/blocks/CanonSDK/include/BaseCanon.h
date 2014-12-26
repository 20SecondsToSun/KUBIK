#pragma once

//#include "cinder/app/AppBasic.h"
//#include "cinder/ImageIo.h"
//#include "cinder/Filesystem.h"
//#include <boost/thread.hpp>

#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"
#include "CanonEventCodes.h"
#include "CanonException.h"
#include "CameraEventListener.h"
#include "CameraController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ci::signals;

namespace canon
{
	namespace resolution
	{
		static const int CAMERA_WIDTH = 1024;
		static const int CAMERA_HEIGHT = 680;
	}

	class BaseCanon 
	{
	public:
		BaseCanon();

		void setup(CameraController* controller, int cameraIndex = 0);
		void shutdown();
		void extendShutDownTimer();

		void startLiveView();
		void endLiveView();			
		void downloadData();

		Surface8u getLiveSurface() const;
		bool isCameraConnected()  const;		
		bool isLiveViewing() const;
		bool isBusy() const;
		bool isFrameNew() const;

		void setFrameNew(bool value);
		void setImageQuality(EdsImageQuality quality = EdsImageQuality_S2JF);
		void setWhiteBalance(EdsWhiteBalance m_whiteBalance = kEdsWhiteBalance_Auto);
		void setListeners();

		int getNumConnectedCameras() const;		

	protected:	
		void takePicture();
		Surface8u mLivePixels;
		bool _isCameraConnected, _isLiveView, _isBusy, _isFrameNew, _isRunning;
		void init(CameraController* controller);
	
		void getDeviceInfo(EdsCameraRef cam);
		EdsError sendCommand(EdsCameraRef inCameraRef, EdsUInt32 inCommand, EdsUInt32 inParam);
		EdsError downloadEvfData(EdsCameraRef camera);

		EdsCameraRef	 camera;
		EdsCameraListRef cameraList;

	private:
		connection shutCon;
	};
}