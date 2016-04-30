#include "QrCodeCreator.h"

using namespace kubik;

ci::gl::Texture QrCodeCreator::generateQRcodeByURL(const std::string& url, const std::string& outFilePath)
{
	pQRC = QRcode_encodeString(url.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);

	if (pQRC)
	{
		auto unWidth = pQRC->width;
		auto unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;

		if (unWidthAdjusted % 4)
		{
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		}

		auto unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

		unsigned char* pRGBData, *pSourceData, *pDestData;

		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			//printf("Out of memory");
			//exit(1);
			return ci::gl::Texture();
		}

		memset(pRGBData, 0xff, unDataBytes);

		// Подготовка заголовков bmp
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
		kInfoHeader.biHeight = -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

		using namespace ci;
		
		// Конвертирование битов QrCode в bmp пиксели
		pSourceData = pQRC->data;
		for (int y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
			for (int x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
					for (int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
						for (int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
						{
							*(pDestData + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
						}
				pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
				pSourceData++;
			}
		}

		FILE* file;
		if (!(fopen_s(&file, outFilePath.c_str(), "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, file);

			fclose(file);
		}
		else
		{
			//printf("Unable to open file");
			//exit(1);
		}

		free(pRGBData);
		QRcode_free(pQRC);
		auto tex = 	gl::Texture(ci::loadImage(outFilePath));
		return tex;
	}
	else
	{
		//	printf("NULL returned");
		//	exit(1);
	}

	return ci::gl::Texture();
}