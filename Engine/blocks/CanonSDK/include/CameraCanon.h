#pragma once

//#include "cinder/app/AppBasic.h"
//#include "cinder/gl/gl.h"
//#include "cinder/gl/Texture.h"
#include "BaseCanon.h"
#include "Utils.h"

using namespace ci;
using namespace ci::gl;
using namespace std;
using namespace boost::signals2;
using namespace canon;

namespace canon
{
	class CameraCanon :public BaseCanon
	{
	public:
		CameraCanon();

		void setup();
		void shutdown();
		void connect();
		void update();
		void draw(Rectf drawingRect = Rectf(0.0f, 0.0f, 0.0f, 0.0f));

		void reset();
		void stop(); 

		void startLiveView();
		void endLiveView();
		void toggleLiveView();

		void stopRecording();
		void startRecording();
		void toggleRecording();    
		void takePicture();

		bool isRecording() const;

		bool checkNewFrame();
		int getWidth() const;
		int getHeight() const;
		int getTotalCapturedFrames() const;
		const vector<Surface>& getCapturedFrames() const;

		void photoTaken(EdsDirectoryItemRef directoryItem);
		void photoDownloaded(const string& downloadPath);
		void photoCameraError(EdsError err);
		void handleStateEvent(EdsUInt32 inEvent);

		fs::path photoDownloadDirectory();
		void photoCameraReadyLiveView();

		signal<void(const string&)> photoDownloadedEvent;
		signal<void(void)>	photoTakenEvent, photoErrorEvent;	
		signal<void(void)> deviceConnectEvent, deviceDisconnectEvent;

		void setDownloadDirectory(fs::path dir);
		bool isConnected();

		ci::gl::Texture getTexture(int sizex, int sizey, int offsetx, int offsety, float scale);

	protected:
		CameraController* controller;

		bool _isRecording, _isFrameNew, _isBusy, isAspectsCompute;   
		int recordingFPS;
		int connectionState;
		int liveViewState;
		double startTime;   

		vector<Surface> mCapturedFrames;   
		Timer reconnectTimer, restartLiveViewTimer;
		enum states {UNDEFINED, CONNECT, DISCONNECT, LIVE, NOLIVE};	

		void saveFrame();
		void pushFrame(const Surface& frame);
		void calculateAspects();	
	};
}