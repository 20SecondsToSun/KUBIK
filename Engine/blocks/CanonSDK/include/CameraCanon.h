#pragma once

#include "BaseCanon.h"
#include "Utils.h"

using namespace ci;
using namespace ci::gl;
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
		void setAutoReconnect(bool autoReconnect = true);
		void update();
		void draw(ci::Rectf drawingRect = ci::Rectf(ci::Vec2f::zero(), ci::Vec2f::zero()));

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
		const vector<ci::Surface>& getCapturedFrames() const;

		void photoTaken(EdsDirectoryItemRef directoryItem);
		void photoDownloadHandler(const std::string& downloadPath);
		void shutdownHandler();

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

		std::vector<ci::Surface> mCapturedFrames;   
		Timer reconnectTimer, restartLiveViewTimer;
		enum states {UNDEFINED, CONNECT, DISCONNECT, LIVE, NOLIVE};	

		void saveFrame();
		void pushFrame(const ci::Surface& frame);
		void calculateAspects();	

	private:
		void autoReconnectCheckUpdate();
		connection autoReconnectSignal;
	};
}