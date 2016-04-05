#pragma once

#include "libqrencode/qrencode.h"
#include "cinder/ImageIo.h"

namespace kubik
{
	class QrCodeCreator
	{
		QRcode* pQRC;
		static const int OUT_FILE_PIXEL_PRESCALER = 12;
		static const unsigned char PIXEL_COLOR_R = 0;
		static const unsigned char PIXEL_COLOR_G = 0;
		static const unsigned char PIXEL_COLOR_B = 0;

	public:
		ci::gl::Texture generateQRcodeByURL(const std::string& url = "http://familyagency.ru", const std::string& outFilePath = "qrcode.bmp");
	};
}
