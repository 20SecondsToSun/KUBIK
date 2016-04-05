#include "states/sharing/QrCode.h"

using namespace kubik::games::photobooth;

QrCode::QrCode() : SimpleSpriteButton(Rectf(0.0, 0.0f, 424.0f, 424.0f))
{

}

void QrCode::drawLayout()
{
	if (qrCode)
	{
		gl::color(ColorA(color.r, color.g, color.b, alpha));
		gl::pushMatrices();
		gl::translate(0.0f, 60.0f);
		gl::drawSolidRect(buttonArea);
		gl::popMatrices();
		gl::draw(title, position);

		gl::pushMatrices();
		gl::translate(0.0f, 60.0f);
		gl::translate((buttonArea.getWidth() - qrCode.getWidth()) * 0.5f, (buttonArea.getHeight() - qrCode.getHeight()) * 0.5f);
		gl::draw(qrCode);
		gl::popMatrices();
		gl::color(Color::white());

		Sprite::drawLayout();
	}
}

void QrCode::initLink(const std::string& link, const std::string& outPath)
{
	QrCodeCreator qr;
	qrCode = qr.generateQRcodeByURL(link, outPath);
}

void QrCode::setTitle(const ci::gl::Texture& texture)
{
	title = texture;
}

void QrCodeNull::drawLayout()
{

}
