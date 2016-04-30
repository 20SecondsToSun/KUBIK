#pragma once

#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"
#include "CanonEventCodes.h"
#include "CanonException.h"
#include "CameraEventListener.h"
#include "CameraController.h"

namespace canon
{
	namespace resolution
	{
		static const int CAMERA_WIDTH = 1056;
		static const int CAMERA_HEIGHT = 704;
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

		ci::Surface8u getLiveSurface() const;
		bool isCameraConnected()  const;	
		void setConnection(bool value);
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
		bool _isCameraConnected, _isLiveView, _isBusy, _isFrameNew, _isRunning;
		void init(CameraController* controller);
	
		void getDeviceInfo(EdsCameraRef cam);
		EdsError sendCommand(EdsCameraRef inCameraRef, EdsUInt32 inCommand, EdsUInt32 inParam);
		EdsError downloadEvfData(EdsCameraRef camera);

		EdsCameraRef	 camera;
		EdsCameraListRef cameraList;

		ci::Surface8u mLivePixels;

	private:
		ci::signals::connection shutCon;
	};
}
