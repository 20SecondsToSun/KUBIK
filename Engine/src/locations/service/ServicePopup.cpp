#include "ServicePopup.h"

using namespace kubik;

ServicePopup::ServicePopup()
{
	string path = getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf";// TODO FONT in Resource
	DataSourceRef ref = loadFile(path);
	font = Font(ref, 30);
}

void ServicePopup::draw()
{
	gl::color(Color::white());
	gl::drawSolidRect(getWindowBounds());
	gl::draw(textTex, textTexPos);
}

void ServicePopup::init(ISettingsRef config)
{

}

void ServicePopup::reset()
{

};

void ServicePopup::setMessage(const char* msg)
{
	this->msg = string(msg);

	textTex = textTools().getTextField(msg, &font, ColorA(1.0f, 0.0f, 0.0f, 1.0f));

	float shiftX = (getWindowWidth() - textTex.getWidth()) * 0.5f;
	float shiftY = (getWindowHeight() - textTex.getHeight()) * 0.5f;
	textTexPos = Vec2f(shiftX, shiftY);
}