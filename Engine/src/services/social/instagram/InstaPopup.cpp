#include "instagram/InstaPopup.h"

using namespace kubik;
using namespace instagram;
using namespace std;
using namespace ci;
using namespace ci::app;

InstaPopup::InstaPopup(InstagramClientRef client, const gl::Texture& close, const gl::Texture& print, const gl::Texture& _template)
	:client(client),
	bgColor(Color::hex(0x060a0e)),
	alpha(0),
	showing(false),
	templateImage(_template)
{
	setDesignElements(close, print, _template);
}

void InstaPopup::setDesignElements(const gl::Texture& close, const gl::Texture& print, const gl::Texture& _template)
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(close, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - close.getSize() * 0.5f));

#ifdef PORTRAIT_RES
	auto positionPrint = Vec2f(0.5f * (getWindowWidth() - print.getWidth()), 1456.0f - print.getHeight()*0.5f);
#else
	auto positionPrint = Vec2f(0.5f * (getWindowWidth() - print.getWidth()), 10.0f);
#endif

	printBtn = ImageButtonSpriteRef(new ImageButtonSprite(print, positionPrint));

	imageShift = Vec2f(0.5f * (getWindowWidth() - 640.0f), 224.0f);

	templateImage = _template;
}

void InstaPopup::draw()
{
	if (!showing)
	{
		return;
	}		

	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::color(ColorA(bgColor.r, bgColor.g, bgColor.b, alpha));
	gl::drawSolidRect(getWindowBounds());
	gl::color(ColorA(1.0f, 1.0f, 1.0f, 1));

	ci::gl::Texture tex = image.getStandartResImage(), _image;
	float templateScale = 1, imageScale = 1;

	if (tex)
	{
		_image = tex;
	}
	else
	{
		auto lowResImage = image.getLowResImage();
		if (lowResImage)
		{			
			_image = lowResImage;
		}			
	}

	if (_image)
	{
		templateScale = 640.0f / templateImage.getWidth();
		imageScale = 640.0f / _image.getWidth();

		gl::pushMatrices();
		gl::translate(imageShift);
		gl::translate(imagePositionAnim);
		gl::scale(templateScale, templateScale);
		gl::draw(templateImage);
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(imageShift);
		gl::translate(imagePositionAnim);
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

	timeline().apply(&alpha, 0.0f, 0.95f, time, eFunc)
		.finishFn(bind(&InstaPopup::showAnimationFinish, this));

	timeline().apply(&imagePositionAnim, Vec2f(0.0f,-100.0f), Vec2f::zero(), time, eFunc);	
}

void InstaPopup::hide(const ci::EaseFn& eFunc, float time)
{
	showing = false;
	timeline().apply(&alpha, 0.0f, time, eFunc)		
		.finishFn(bind(&InstaPopup::hideAnimationFinish, this));
}

void InstaPopup::showAnimationFinish()
{

}

void InstaPopup::hideAnimationFinish()
{

}

void InstaPopup::setAlpha(float alpha)
{

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