#include "states/sharing/social/TwitterPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

TwitterPopup::TwitterPopup(PhotoboothSettingsRef settings)
	:Popup(settings)
{

}

void TwitterPopup::show()
{
	Popup::show();
}

void TwitterPopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void TwitterPopup::showAnimComplete()
{
	touchKeyboard().connectEventHandler(&TwitterPopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
	Popup::showAnimComplete();	
}

void TwitterPopup::keyTouchHandler()
{
	console() << "::::::touch " << touchKeyboard().getLastCode() << endl;
}

void TwitterPopup::hide(EventGUIRef& event)
{
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
	Popup::hide(event);
}

void TwitterPopup::draw()
{
	Popup::draw();	
}