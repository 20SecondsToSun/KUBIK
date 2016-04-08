#include "ServicePopup.h"
#include "Paths.h"

using namespace kubik;
using namespace kubik::config;
using namespace std;
using namespace ci;
using namespace ci::app;

ServicePopup::ServicePopup()
{
	DataSourceRef ref = loadFile(Paths::getPopupFontPath());
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

}

void ServicePopup::setMessage(const char* msg)
{
	this->msg = string(msg);

	textTex = textTools().getTextField(msg, &font, ColorA(1.0f, 0.0f, 0.0f, 1.0f));

	float shiftX = (getWindowWidth()  - textTex.getWidth())  * 0.5f;
	float shiftY = (getWindowHeight() - textTex.getHeight()) * 0.5f;
	textTexPos = Vec2f(shiftX, shiftY);
}