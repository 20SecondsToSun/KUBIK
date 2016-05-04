#pragma once

#include <winspool.h>
//#define DMPAPER_BOOTH_TWO_PHOTOS               515    /* Letter  5x15 type 2              */
#define DMPAPER_BOOTH_ONE_PHOTO  501  /* Letter  10x15(4x6'')         */
#define DMPAPER_BOOTH_TWO_PHOTOS  514  /* Letter  10x15(4x6'')         */

namespace kubik
{
	class Printer
	{
	public:
		static Printer& getInstance();

		//LPWSTR printerName = L"MITSUBISHI CP70D Series(USB)";
		LPWSTR printerName = L"MITSUBISHI CP-K60DW-S";		

		LPWSTR imagePath = L"XXXXXX";
		char * imagePath_c;

		//short printerLetterSize = DMPAPER_BOOTH_TWO_PHOTOS;
		short printerLetterSize = DMPAPER_BOOTH_ONE_PHOTO;
		short printerLetterOrientation = DMORIENT_PORTRAIT;

		int applySettings();
		void applyInstagramSettings();
		void applyPhotoBoothSettings(short printerLetterSize);

		void setPathToPrintImage(LPWSTR value);
		int print();
		//void saveData(const std::string& fullPath, const std::string& saveString);

		ci::Vec2f getPixelsSize() const
		{
			//return ci::Vec2f(1205, 1795);
			//return ci::Vec2f(1181, 1772);
			//return ci::Vec2f(1218, 1864);
			return ci::Vec2f(1218, 1864);
		}

		void GetPrinterList();

		//private:

	};

	inline Printer& printer()
	{
		return Printer::getInstance();
	};
}