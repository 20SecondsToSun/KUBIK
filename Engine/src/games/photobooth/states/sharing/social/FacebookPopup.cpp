#include "states/sharing/social/FacebookPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

FacebookPopup::FacebookPopup(PhotoboothSettingsRef settings)
	:Popup(settings)
{

}

void FacebookPopup::show()
{
	Popup::show();
}

void FacebookPopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void FacebookPopup::showAnimComplete()
{
	touchKeyboard().connectEventHandler(&FacebookPopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
	Popup::showAnimComplete();	
}

void FacebookPopup::keyTouchHandler()
{
	console() << "::::::touch " << touchKeyboard().getLastCode() << endl;
}

void FacebookPopup::hide(EventGUIRef& event)
{
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
	Popup::hide(event);
}

void FacebookPopup::draw()
{
	Popup::draw();	
}