#include "services/printer/Printer.h"
#include "Utils.h"
#include "Paths.h"
#include "atlstr.h"

using namespace kubik;
using namespace kubik::config;
using namespace std;
using namespace ci;

Printer& Printer::getInstance()
{
	static Printer base;
	return base;
}

void Printer::setPathToPrintImage(LPWSTR value)
{
}

void Printer::applyInstagramSettings()
{
	printerLetterSize = DMPAPER_BOOTH_ONE_PHOTO;
	printer().applySettings();
	string path = Paths::getPrintPathString();
	string photo_url = Utils::twiceSymbol(path, '\\');

	char* cstr = _strdup(photo_url.c_str());
	printer().imagePath_c = cstr;
}

void Printer::applyPhotoBoothSettings(short _printerLetterSize)
{
	printerLetterSize = _printerLetterSize;
	printer().applySettings();
	string path = Paths::getPrintPathString();
	string photo_url = Utils::twiceSymbol(path, '\\');

	char* cstr = _strdup(photo_url.c_str());
	printer().imagePath_c = cstr;
}

int Printer::applySettings()
{
	HANDLE print_handle;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	DWORD dwNeeded = 0;
	DEVMODE *pDevMode = NULL;
	LONG lFlag;


	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = PRINTER_ALL_ACCESS;

	bFlag = OpenPrinter(printerName, &print_handle, &pd);

	if (!bFlag || (print_handle == NULL))
	{
		//console()<< "::::::::::::::::  OpenPrinter ERROR ::::::::::::::  "<<std::endl;
		return -1;
	}

	bFlag = GetPrinter(print_handle, 2, 0, 0, &dwNeeded);


	if ((!bFlag) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER) ||
		(dwNeeded == 0))
	{
		ClosePrinter(print_handle);
		//console()<< ":::::::::::::::  SOMETHING GONE WRONG ::::::::::::::  "<<std::endl;
		return -1;
	}

	PRINTER_INFO_2 *pi2 = (PRINTER_INFO_2  *)::GlobalAlloc(GPTR, dwNeeded);

	if (pi2 == NULL)
	{
		ClosePrinter(print_handle);
		//console()<< " :::::::::::::::::: NOT MEMORY ALLOCATE :::::::::::::::::  "<<std::endl;
		return  -1;
	}

	bFlag = GetPrinter(print_handle, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag)
	{
		GlobalFree(pi2);
		ClosePrinter(print_handle);
		//console()<< ":::::::::::::::  SOMETHING GONE WRONG ::::::::::::::  "<<std::endl;
		return -1;
	}

	if (pi2->pDevMode == NULL)
	{
		dwNeeded = DocumentProperties(NULL, print_handle, printerName,	NULL, NULL, 0);

		if (dwNeeded <= 0)
		{
			GlobalFree(pi2);
			ClosePrinter(print_handle);
			return -1;
		}

		pDevMode = (DEVMODE *)GlobalAlloc(GPTR, dwNeeded);
		if (pDevMode == NULL)
		{
			GlobalFree(pi2);
			ClosePrinter(print_handle);
			return -1;
		}

		lFlag = DocumentProperties(NULL, print_handle,	printerName, pDevMode, NULL, DM_OUT_BUFFER);

		if (lFlag != IDOK || pDevMode == NULL)
		{
			GlobalFree(pDevMode);
			GlobalFree(pi2);
			ClosePrinter(print_handle);
			return -1;
		}

		pi2->pDevMode = pDevMode;
	}

	// check that the driver supports the changes 
	if (pi2->pDevMode->dmFields & DM_PAPERSIZE)
	{
		ci::app::console() << "  pi2->pDevMode->dmPaperSize  " << pi2->pDevMode->dmPaperSize << std::endl;
		// define the page size as A3
		pi2->pDevMode->dmPaperSize = printerLetterSize;
		// define, which field was changed
		pi2->pDevMode->dmFields |= DM_PAPERSIZE;
	}
	//console()<< " pi2->pDevMode->dmPaperSize   changed ::  "<< pi2->pDevMode->dmPaperSize<<std::endl;

	/*
	*	Make changes to the DevMode which are supported.
	*/
	//console()<< " pi2->pDevMode->dmOrientation  ::  "<< pi2->pDevMode->dmOrientation<<std::endl;
	if (pi2->pDevMode->dmFields & DM_ORIENTATION)
	{
		/* If the printer supports paper orientation, set it.*/
		pi2->pDevMode->dmOrientation = printerLetterOrientation;// DMORIENT_LANDSCAPE;
		pi2->pDevMode->dmFields |= DM_ORIENTATION;

	}
	/*if (pi2->pDevMode->dmCopies & DM_COPIES)
	{
	pi2->pDevMode->dmCopies = 1;
	pi2->pDevMode->dmCopies |= DM_COPIES;
	}*/
	/*console()<< " pi2->pDevMode->dmOrientation changed ::  "<< pi2->pDevMode->dmOrientation<<std::endl;
	console()<< " pi2->pDevMode->dmScale ::  "<< pi2->pDevMode->dmScale<<std::endl;
	console()<< " pi2->pDevMode->dmPanningHeight ::  "<< pi2->pDevMode->dmPanningHeight<<std::endl;
	console()<< " pi2->pDevMode->dmCopies ::  "<< pi2->pDevMode->dmCopies<<std::endl;
	console()<< " pi2->pDevMode->dmPaperLength ::  "<< pi2->pDevMode->dmPaperLength<<std::endl;
	console()<< " pi2->pDevMode->dmPaperWidth ::  "<< pi2->pDevMode->dmPaperWidth<<std::endl;
	console()<< " pi2->pDevMode->dmPaperSize ::  "<< pi2->pDevMode->dmPaperSize<<std::endl;
	console()<< " pi2->pDevMode->dmPelsWidth ::  "<< pi2->pDevMode->dmPelsWidth<<std::endl;
	console()<< " pi2->pDevMode->dmPelsHeight ::  "<< pi2->pDevMode->dmPelsHeight<<std::endl;*/



	//lpMargins = (LPDWORD)( (LPBYTE) pi2->pDevMode +  pi2->pDevMode->dmSize + sizeof( DWORD ) );
	//*lpMargins = MAKELONG( 30, 30 ); // this will set the margins to 3 mm


	// Do not attempt to set security descriptor...
	pi2->pSecurityDescriptor = NULL;


	// Make sure the driver-dependent part of devmode is updated...
	lFlag = DocumentProperties(NULL, print_handle, printerName,	pi2->pDevMode, pi2->pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);

	if (lFlag != IDOK)
	{
		//console()<< "  ERROR!!! not updated !!!!!!!!!!!!!!!!!!!!! ::  "<<std::endl;
		GlobalFree(pi2);
		ClosePrinter(print_handle);
		if (pDevMode)
			GlobalFree(pDevMode);
		return -1;
	}

	// Update printer information...
	bFlag = SetPrinter(print_handle, 2, (LPBYTE)pi2, 0);

	if (!bFlag)
		// The driver doesn't support, or it is unable to make the change...
	{
		//console()<< "  ERROR!!! not updated !!!!!!!!!!!!!!!!!!!!! ::  "<<std::endl;
		GlobalFree(pi2);
		ClosePrinter(print_handle);
		if (pDevMode)
			GlobalFree(pDevMode);
		return -1;
	}

	// Tell other apps that there was a change...
	SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L, (LPARAM)(LPCSTR)printerName, SMTO_NORMAL, 1000, NULL);

	return 1;
}

int Printer::print()
{
	DWORD	 dwError = 0;

	TCHAR	 szJobName[128] = { 0 };
	HDC	 hDC = 0;
	DOCINFO	 DocInfo;

	BITMAPINFO	 BmpInfo;
	int	 nRet = 0;

	HBITMAP	 hBmp24 = 0;
	BITMAP	 Bmp24 = { 0 };

	LOGFONT	 LogFont = { 0 };
	HFONT	 hFont = 0,
		hDefFont = 0;

	ZeroMemory(&hBmp24, sizeof(HBITMAP));


	wchar_t wtext[200];
	mbstowcs(wtext, imagePath_c, strlen(imagePath_c) + 1);//Plus null		
	imagePath = wtext;


	hBmp24 = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_VGACOLOR);

	if (!hBmp24)
	{
		ci::app::console() << "hBmp  ERROR  " << dwError << std::endl;
		return -1;
	}
	else
	{
		ci::app::console() << "hBmp  LOADED to PRINT!!!  " << dwError << std::endl;
	}

	dwError = GetLastError();
	GetObject(hBmp24, sizeof(BITMAP), &Bmp24);

	hDC = CreateDC(L"WINSPOOL", printerName, NULL, 0);

	//4. StartDoc
	memset(&DocInfo, 0, sizeof(DOCINFO));
	DocInfo.cbSize = sizeof(DOCINFO);

	lstrcpy(szJobName, L"Photo");
	DocInfo.lpszDocName = szJobName;

	int nJobId = ::StartDoc(hDC, &DocInfo);

	//5. StartPage
	nRet = ::StartPage(hDC);

	//6.b Draw anything you want on page between ::StartPage and ::EndPage
	if (Bmp24.bmBits)
	{
		memset(&BmpInfo, 0, sizeof(BITMAPINFO));
		BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BmpInfo.bmiHeader.biWidth = Bmp24.bmWidth;
		BmpInfo.bmiHeader.biHeight = Bmp24.bmHeight;
		BmpInfo.bmiHeader.biPlanes = Bmp24.bmPlanes;
		BmpInfo.bmiHeader.biBitCount = Bmp24.bmBitsPixel * Bmp24.bmPlanes;
		BmpInfo.bmiHeader.biCompression = BI_RGB;

		nRet = StretchDIBits(hDC, 0, 0, Bmp24.bmWidth, Bmp24.bmHeight, 0, 0, Bmp24.bmWidth, Bmp24.bmHeight, Bmp24.bmBits, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	else return -1;

	//7. If page drawing is finished, set to end page
	nRet = ::EndPage(hDC);

	//8. If job done, set end of the job
	nRet = ::EndDoc(hDC);
	//release image object
	if (hBmp24)
		DeleteObject(hBmp24);

	DeleteDC(hDC);

	return 1;
}

void Printer::GetPrinterList()
{
//#if defined(__WXMSW__) 

	// When Name is NULL, setting Flags to PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS 
	// enumerates printers that are installed on the local machine. These printers include 
	// those that are physically attached to the local machine as well as remote printers to 
	// which it has a network connection. 

	DWORD dwFlags = PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL;
	LPTSTR Name = NULL;
	DWORD dwNeeded = 0;
	DWORD dwReturned = 0;

	// First, find out how much space we need for the return buffers 
	if (::EnumPrinters(dwFlags, Name, 4, 0, 0, &dwNeeded, &dwReturned))
	{
		return;
		//throw CLogicError(ERR_POS);
	}

	DWORD LastError = ::GetLastError();
	if (LastError != ERROR_INSUFFICIENT_BUFFER)
	{
		return;
		//throw CLogicError(ERR_POS);
	}

	// Allocate the buffer 
	PRINTER_INFO_4 * PrinterInfo = (PRINTER_INFO_4 *)malloc(dwNeeded);

	if (!PrinterInfo)
	{
		return;
		//throw CLogicError(ERR_POS);
	}

	if (!::EnumPrinters(dwFlags, Name, 4, (unsigned char *)PrinterInfo, dwNeeded, &dwNeeded, &dwReturned)) 
	{
		return; 
		//throw CLogicError(ERR_POS);
	}

	logger().log("!!!!!!!!!!!!!!!!!!!!!!!!!!" + toString(dwReturned));
	wchar_t printer_name[MAX_PATH];
	DWORD size = 260;// arraysize(printer_name);
	BOOL result = ::GetDefaultPrinter(printer_name, &size);
	printf("wchar_t string: %ls \n", printer_name);
	//logger().log("!!!!!!!!!!!!szPrinterName!!!!!!!!!!!!!!" + toString(printer_name));


	PRINTER_DEFAULTS pd;
	HANDLE print_handle;

	for (int i = 0; i < (int)dwReturned; i++) 
	{
		ZeroMemory(&pd, sizeof(pd));
		pd.DesiredAccess = PRINTER_ALL_ACCESS;

		bool bFlag = OpenPrinter(printerName, &print_handle, &pd);

		string printerNameString = CW2A(PrinterInfo[i].pPrinterName);
		logger().log("printer name " + printerNameString);

		if (!bFlag || (print_handle == NULL))
		{
			ci::app::console()<< "::::::::::::::::  OpenPrinter ERROR ::::::::::::::  "<<std::endl;			
		}
		else
		{
			bFlag = GetPrinter(print_handle, 2, 0, 0, &dwNeeded);

			if ((!bFlag) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER) ||	(dwNeeded == 0))
			{
				ClosePrinter(print_handle);
				ci::app::console() << ":::::::::::::::  SOMETHING GONE WRONG ::::::::::::::  " << std::endl;
				
			}
			else
			{
				ci::app::console() << "::::::::::::::::  printer OK ::::::::::::::  " << std::endl;
				break;
			}
		}
	}

	free(PrinterInfo);
//#endif 
}