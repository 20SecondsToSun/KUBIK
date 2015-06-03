#pragma once 

//#include "cinder/app/AppBasic.h"
//#include "cinder/Filesystem.h"

#include "EDSDK.h"
#include "CanonEventCodes.h"
#include "CanonException.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ci::signals;
using namespace fs;

namespace canon
{
	class CameraController
	{
	public:
		signal<void(string)>		photoDownloadedSignal;
		signal<void(string)>		photoDownloadedErrorSignal;
		signal<void()>				cameraShutDownSignal;		

		void perform(canonEvent evt);
		void downloadImage(EdsDirectoryItemRef dirItem);

		path getDownloadedDir();
		void setDownloadedDir(fs::path dir);

	private:
		path downloadDir;
		path downloadFile;

		void _downloadImage(EdsDirectoryItemRef dirItem);
	};
}