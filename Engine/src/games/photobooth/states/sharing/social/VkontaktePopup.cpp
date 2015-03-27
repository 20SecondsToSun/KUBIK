#include "states/sharing/social/VkontaktePopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

VkontaktePopup::VkontaktePopup(PhotoboothSettingsRef settings)
	:Popup(settings)
{

}

void VkontaktePopup::show()
{
	Popup::show();
}

void VkontaktePopup::initVirtualKeyboard()
{
	touchKeyboard().setInputFieldVisible(false);
	Popup::initVirtualKeyboard();
}

void VkontaktePopup::showAnimComplete()
{
	touchKeyboard().connectEventHandler(&VkontaktePopup::keyTouchHandler, this, VirtualKeyboard::KEY_TOUCH);
	Popup::showAnimComplete();	
}

void VkontaktePopup::keyTouchHandler()
{
	console() << "::::::touch " << touchKeyboard().getLastCode() << endl;
}

void VkontaktePopup::hide(EventGUIRef& event)
{
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::KEY_TOUCH);
	Popup::hide(event);
}

void VkontaktePopup::draw()
{
	Popup::draw();	
}