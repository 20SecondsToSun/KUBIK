#include "main/social/EmailPopup.h"
#include "server/Server.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;
using namespace ci;
using namespace ci::app;

EmailPopup::EmailPopup(ISettingsRef settings, kubik::config::StatCollectorRef statSettings)
	:Popup(settings),
	set(settings),
	statSettings(statSettings),
	addEmailFontColor(Color::white()),
	addEmailIcon(settings->getTexture("addEmail")),
	borderIcon(settings->getTexture("enterEmailBorder")),
	borderIconRed(settings->getTexture("errorEmailBorder")),
	addEmailFont(getFont("Helvetica Neue Light", 24)),
	initAddEmailPosition(Vec2f(165.0f, 480.0f)),
	shiftEmailPosition(Vec2f(0.0f, 63.0f))
{
	borderIconPos = Vec2f((getWindowWidth() - borderIcon.getWidth()) * 0.5f, 350.0f - borderIcon.getHeight() * 0.5f);

	addEmailBtn = ImageButtonSpriteRef(new ImageButtonSprite(addEmailIcon, 
		Vec2f(borderIconPos.x + borderIcon.getWidth() - addEmailIcon.getWidth(), 
		500.0f - addEmailIcon.getHeight() * 0.5f)));
}

void EmailPopup::setPhotoID(int _photo_id)
{
	photo_id = _photo_id;
}

void EmailPopup::show()
{
	
	showAddEmail = true;
	clearEmails();
	Popup::show();
}

void EmailPopup::clearEmails()
{
	emails.clear();
	emailsTextures.clear();
}

void EmailPopup::initVirtualKeyboard()
{
	touchKeyboard().setInputField(borderIconPos.x + 10, borderIconPos.y, borderIconPos.x + 810.0f, borderIconPos.y + 139.0f);
	touchKeyboard().setInputFont(inputFont);
	touchKeyboard().setInputColor(Color::white());
	touchKeyboard().setDrawingCarriage(true);

	Popup::initVirtualKeyboard();
}

void EmailPopup::showAnimComplete()
{
	Popup::showAnimComplete();
	addEmailBtn->connectEventHandler(&EmailPopup::addEmailHandler, this);
	touchKeyboard().connectEventHandler(&EmailPopup::sendEmailHandler, this, VirtualKeyboard::SEND_TOUCH);
}

void EmailPopup::hide(EventGUIRef& event)
{
	addEmailBtn->disconnectEventHandler();
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
	Popup::hide(event);
}

void EmailPopup::addEmailHandler(EventGUIRef& event)
{
	if (handleInputField())
	{
		addEmailToList();
	}		
}

bool EmailPopup::handleInputField()
{
	if (!Utils::validate_email(touchKeyboard().getInputFieldText()))
	{
		showRedFocusStroke();
		return false;
	}

	return true;
}

void EmailPopup::addEmailToList()
{
	auto text = touchKeyboard().getInputFieldText();
	emails.push_back(text);
	touchKeyboard().clearInputFieldText();

	EmailTex email;
	email.texture = textTools().getTextField(text, &addEmailFont, addEmailFontColor);
	email.position = initAddEmailPosition + emailsTextures.size() * shiftEmailPosition;
	emailsTextures.push_back(email);

	if (emails.size() == MAX_EMAIL_TO_SEND)
	{
		addEmailBtn->setAlpha(0.0f);
		addEmailBtn->disconnectEventHandler();
		showAddEmail = false;
	}
}

void EmailPopup::showRedFocusStroke()
{
	timeline().apply(&alphaError, 1.0f, 0.0f, 2.5f, EaseOutCubic());
}

void EmailPopup::draw()
{
	Popup::drawBackgrounds();
	Popup::draw();

	drawInputField();

	if (showAddEmail)
	{
		addEmailBtn->setAlpha(alphaAnim);
		addEmailBtn->draw();
	}

	drawAddedEmails();	

	gl::color(Color::white());
}

void EmailPopup::drawInputField()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(borderIcon, borderIconPos);
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaError));
	gl::draw(borderIconRed, borderIconPos);
}

void EmailPopup::drawAddedEmails()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));

	for (auto email : emailsTextures)
	{
		gl::draw(email.texture, email.position);
	}
}

void EmailPopup::sendEmailHandler()
{
	if (!Utils::validate_email(touchKeyboard().getInputFieldText()) && emails.size() == 0)
	{
		showRedFocusStroke();
		return;
	}	

	// if everything ok send mail

	/*	
		sending to server	
	*/

	server().sendToEmails(2, photo_id, getEmailInString());
	statSettings->addEmailShare();	
 	shareCompleteSignal(SharingType::EMAIL, getEmailInString());


	addEmailBtn->disconnectEventHandler();
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
	Popup::hide(event);
}

std::string EmailPopup::getEmailInString()
{
	std::string out = touchKeyboard().getInputFieldText();
	if (out != "" && emails.size())
	{
		out += ",";

		for (size_t i = 0; i < emails.size(); i++)
		{
			out += emails[i];

			if (i != (emails.size() - 1))
			{
				out += ",";
			}
		}
	}

	

	

	return out;
}