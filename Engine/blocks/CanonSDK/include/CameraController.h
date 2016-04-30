#pragma once 

#include "EDSDK.h"
#include "CanonEventCodes.h"
#include "CanonException.h"

namespace canon
{
	class CameraController
	{
	public:
		ci::signals::signal<void(std::string)>		photoDownloadedSignal;
		ci::signals::signal<void(std::string)>		photoDownloadedErrorSignal;
		ci::signals::signal<void()>					cameraShutDownSignal;
		ci::signals::signal<void()>					wiilSoonShutDownSignal;

		void perform(canonEvent evt);
		void downloadImage(EdsDirectoryItemRef dirItem);
		ci::fs::path getDownloadedDir();
		void setDownloadedDir(ci::fs::path dir);

	private:
		ci::fs::path downloadDir;
		ci::fs::path downloadFile;

		void _downloadImage(EdsDirectoryItemRef dirItem);
	};
}
