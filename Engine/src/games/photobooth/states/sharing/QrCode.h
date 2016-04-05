#pragma once

#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"
#include "DrawTools.h"
#include "model/PhotoStorage.h"
#include "Tools/qrCode/QrCodeCreator.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef shared_ptr<class QrCode> QrCodeRef;
			typedef shared_ptr<class QrCodeNull> QrCodeNullRef;
			
			class QrCode :public SimpleSpriteButton
			{
				ci::Vec2f position;
				ci::gl::Texture title, qrCode;

			public:
				QrCode();

				virtual void drawLayout() override;
				virtual void initLink(const std::string& link = "", const std::string& outPath = "");
				virtual void setTitle(const ci::gl::Texture& texture);				
			};

			class QrCodeNull : public QrCode
			{
				virtual void drawLayout();
			};
		}
	}
}
