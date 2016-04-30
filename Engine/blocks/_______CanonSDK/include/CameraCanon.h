#pragma once

#include "BaseCanon.h"
#include "Utils.h"

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
		const std::vector<ci::Surface>& getCapturedFrames() const;

		void photoTaken(EdsDirectoryItemRef directoryItem);
		void photoDownloadHandler(const std::string& downloadPath);
		void shutdownHandler();

		void photoCameraError(EdsError err);
		void handleStateEvent(EdsUInt32 inEvent);

		ci::fs::path photoDownloadDirectory();
		void photoCameraReadyLiveView();

		ci::signals::signal<void(const std::string&)> photoDownloadedEvent;
		ci::signals::signal<void(void)>	photoTakenEvent, photoErrorEvent;
		ci::signals::signal<void(void)> deviceConnectEvent, deviceDisconnectEvent;

		void setDownloadDirectory(ci::fs::path dir);
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
		ci::Timer reconnectTimer, restartLiveViewTimer;
		enum states {UNDEFINED, CONNECT, DISCONNECT, LIVE, NOLIVE};	

		void saveFrame();
		void pushFrame(const ci::Surface& frame);
		void calculateAspects();	

	private:
		void autoReconnectCheckUpdate();
		ci::signals::connection autoReconnectSignal;
	};
}