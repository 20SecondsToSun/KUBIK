#include "CameraController.h"

using namespace canon;

void CameraController::perform(canonEvent evt)
{
	//console()<<"PERFORM:::::::::::::::::::::::  "<<evt<<endl;
}

void CameraController::downloadImage(EdsDirectoryItemRef dirItem)
{	
	try
	{
		_downloadImage(dirItem);
		photoDownloadedSignal(downloadFile.generic_string());	
	}
	catch(...)
	{
		photoDownloadedErrorSignal("erroe");
	}
}

path CameraController::getDownloadedDir()
{
	return downloadDir;

}

void CameraController::setDownloadedDir(path dir)
{
	downloadDir = dir;
}

void CameraController::_downloadImage(EdsDirectoryItemRef dirItem)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;	
	EdsDirectoryItemInfo dir_item_info;

	err = EdsGetDirectoryItemInfo(dirItem, &dir_item_info);
	if(err != EDS_ERR_OK) 
		throw ExcPhotoDownload(CanonErrorToString(err));		

	// Created file stream to download image.
	downloadFile = downloadDir / string(dir_item_info.szFileName);
	string photoFileName = string(dir_item_info.szFileName);

	err = EdsCreateFileStream(downloadFile.generic_string().c_str(),
		kEdsFileCreateDisposition_CreateAlways,
		kEdsAccess_ReadWrite,
		&stream);	

	if(err != EDS_ERR_OK) 
		throw ExcPhotoDownload(CanonErrorToString(err));	

	//err = EdsSetProgressCallback(stream, DownloadImageProgress, kEdsProgressOption_Periodically, this);	
	//if(err != EDS_ERR_OK)
	//	throw ExcPhotoDownload();	

	err = EdsDownload(dirItem, dir_item_info.size, stream);

	if(err != EDS_ERR_OK) 
	{
		EdsDownloadCancel(dirItem);	
		throw ExcPhotoDownload(CanonErrorToString(err));	
	}

	console() << "Downloaded image to --- " << downloadFile.generic_string() << "\n";
	err = EdsDownloadComplete(dirItem);			

	if(err != EDS_ERR_OK)
		throw ExcPhotoDownload(CanonErrorToString(err));	

	// Release stream
	if(stream != NULL)
	{
		err = EdsRelease(stream);
		if(err != EDS_ERR_OK)
			throw ExcPhotoDownload(CanonErrorToString(err));	
		stream = NULL;
	}
}	