#include "instagram/InstaPopup.h"

using namespace instagram;
using namespace ci;
using namespace ci::app;

InstaPopup::InstaPopup(InstagramClientRef client, const gl::Texture& close, const gl::Texture& print, const gl::Texture& _template)
	:client(client),
	bgColor(Color::hex(0x060a0e)),
	alpha(0),
	showing(false),
	templateImage(_template)
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(close, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - close.getSize() * 0.5f));
	printBtn = ImageButtonSpriteRef(new ImageButtonSprite(print, Vec2f(0.5f * (getWindowWidth() - print.getWidth()), 10.0f)));

	imageShift = Vec2f(0.5f * (getWindowWidth() - 640.0f), 224.0f);
}

void InstaPopup::draw()
{
	using namespace ci;

	if (!showing)
		return;

	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::color(ColorA(bgColor.r, bgColor.g, bgColor.b, alpha));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());

	ci::gl::Texture tex = image.getStandartResImage(), _image;
	float templateScale = 1, imageScale = 1;

	if (tex)
	{
		_image = tex;
		templateScale = 640.0f / templateImage.getWidth();
	}
	else
	{
		auto lowResImage = image.getLowResImage();
		if (lowResImage)
		{
			templateScale = 640.0f / templateImage.getWidth();
			 imageScale = 640.0f / lowResImage.getWidth();
			_image = lowResImage;
		}			
	}

	if (_image)
	{
		gl::pushMatrices();
		gl::translate(imageShift);
		gl::scale(templateScale, templateScale);
		gl::draw(templateImage);
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(imageShift);
		gl::scale(imageScale, imageScale);
		gl::draw(_image);
		gl::popMatrices();
	}	

	closeBtn->draw();
	printBtn->draw();
	gl::popMatrices();
}

void InstaPopup::show(const ImageGraphic& image, const ci::EaseFn& eFunc, float time)
{
	this->image = image;
	showing = true;
	alpha = 0.0f;
	timeline().apply(&alpha, 0.95f, time, eFunc)
		.updateFn(bind(&InstaPopup::alphAnimationUpdate, this))
		.finishFn(bind(&InstaPopup::showAnimationFinish, this));
}

void InstaPopup::hide(const ci::EaseFn& eFunc, float time)
{
	showing = false;
	timeline().apply(&alpha, 0.0f, time, eFunc)
		.updateFn(bind(&InstaPopup::alphAnimationUpdate, this))
		.finishFn(bind(&InstaPopup::hideAnimationFinish, this));
}

void InstaPopup::alphAnimationUpdate()
{
	setAlpha(alpha.value());
}

void InstaPopup::showAnimationFinish()
{

}

void InstaPopup::hideAnimationFinish()
{
	//HideCompleteSignal();
}

void InstaPopup::setAlpha(float alpha)
{
	//bgColor = Utils::colorAlpha(bgColor, alpha);
	//closeBlock->setAlpha(alpha);
	//CompositeDispatcher::setAlpha(alpha);
}

void InstaPopup::activateListeners()
{
	closeBtn->connectEventHandler(&InstaPopup::hiding, this);
	printBtn->connectEventHandler(&InstaPopup::printing, this);
}

void InstaPopup::unActivateListeners()
{
	closeBtn->disconnectEventHandler();
	printBtn->disconnectEventHandler();
}

void InstaPopup::hiding(EventGUIRef& event)
{
	callback(CLOSE_POPUP);
}

void InstaPopup::printing(EventGUIRef& event)
{
	callback(PRINT);	
}

bool InstaPopup::isOpen()
{
	return showing;
}