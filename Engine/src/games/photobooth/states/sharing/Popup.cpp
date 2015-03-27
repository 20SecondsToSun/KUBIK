#include "states/sharing/Popup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

Popup::Popup(PhotoboothSettingsRef settings)
	:headColor(Color::hex(0x4b515a)),
	bgColor(Color::hex(0x34383f)), 
	addEmailFontColor(Color::white()),
	closeIcon(settings->getTexture("closePopup")),
	addEmailIcon(settings->getTexture("addEmail")),
	borderIcon(settings->getTexture("enterEmailBorder")),
	borderIconRed(settings->getTexture("errorEmailBorder")),
	inputFont(settings->getFont("introBook44")),
	addEmailFont(settings->getFont("helveticaNeueLight24")),
	initAddEmailPosition(Vec2f(165.0f, 480.0f)),
	shiftEmailPosition(Vec2f(0.0f, 63.0f))
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(closeIcon, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - closeIcon.getSize() * 0.5f));
	borderIconPos = Vec2f((getWindowWidth() - borderIcon.getWidth()) * 0.5f, 350.0f - borderIcon.getHeight() * 0.5f);

	addEmailBtn = ImageButtonSpriteRef(new ImageButtonSprite(addEmailIcon, 
		Vec2f(borderIconPos.x + borderIcon.getWidth() - addEmailIcon.getWidth(), 
		500.0f - addEmailIcon.getHeight() * 0.5f)));
}

void Popup::show()
{
	showAddEmail = true;

	clearEmails();

	initVirtualKeyboard();

	timeline().apply(&alphaAnim, 0.0f, 0.97f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::showAnimComplete, this));
}

void Popup::clearEmails()
{
	emails.clear();
	emailsTextures.clear();
}

void Popup::initVirtualKeyboard()
{
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setInputField(borderIconPos.x + 10, borderIconPos.y, borderIconPos.x + 810.0f, borderIconPos.y + 139.0f);
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().setInputFont(inputFont);
	touchKeyboard().setInputColor(Color::white());
	touchKeyboard().connectKeyboard();
	auto endY = 800.0f;
	touchKeyboard().show(Vec2f(30.0f, endY + 500.0f), Vec2f(30.0f, endY), 0.7f);
	//touchKeyboard().connectEventHandler(&NewActivityPopup::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);
}

void Popup::showAnimComplete()
{
	closeBtn->connectEventHandler(&Popup::hide, this);
	addEmailBtn->connectEventHandler(&Popup::addEmailHandler, this);
}

void Popup::hideAnimComplete()
{
	callback(POPUP_CLOSED);
}

void Popup::hide(EventGUIRef& event)
{
	closeBtn->disconnectEventHandler();
	addEmailBtn->disconnectEventHandler();

	touchKeyboard().clearInputFieldText();
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hide(Vec2f(30.0f, 1950.0f), 0.3f);

	timeline().apply(&alphaAnim, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::hideAnimComplete, this));
}

void Popup::addEmailHandler(EventGUIRef& event)
{
	console() << "add email" << endl;
	if (handleInputField())
		addEmailToList();
}

bool Popup::handleInputField()
{
	if (touchKeyboard().emptyInputField())
	{
		showRedFocusStroke();
		return false;
	}

	return true;
}

void Popup::addEmailToList()
{
	auto text = touchKeyboard().getInputFieldText();
	emails.push_back(text);
	touchKeyboard().clearInputFieldText();

	EmailTex email;
	email.texture = textTools().getTextField(text, &addEmailFont, addEmailFontColor);
	email.position = initAddEmailPosition + emailsTextures.size() * shiftEmailPosition;
	emailsTextures.push_back(email);

	if (emails.size() == 3)
	{
		addEmailBtn->setAlpha(0.0f);
		addEmailBtn->disconnectEventHandler();
		showAddEmail = false;
	}
}

void Popup::showRedFocusStroke()
{
	timeline().apply(&alphaError, 1.0f, 0.0f, 2.5f, EaseOutCubic());
}

void Popup::draw()
{
	drawBackgrounds();

	closeBtn->setAlpha(alphaAnim);
	closeBtn->draw();

	drawInputField();

	if (showAddEmail)
	{
		addEmailBtn->setAlpha(alphaAnim);
		addEmailBtn->draw();
	}

	drawAddedEmails();

	touchKeyboard().draw();

	gl::color(Color::white());
}

void Popup::drawBackgrounds()
{
	float height = 716.0f;
	auto color = Utils::colorAlpha(headColor, alphaAnim);
	gl::color(color);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), height));
	color = Utils::colorAlpha(bgColor, alphaAnim);
	gl::color(color);
	gl::pushMatrices();
	gl::translate(0.0f, height);
	gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 1920.0f - height));	
	gl::popMatrices();
}

void Popup::drawInputField()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(borderIcon, borderIconPos);
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaError));
	gl::draw(borderIconRed, borderIconPos);
}

void Popup::drawAddedEmails()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	for (auto email : emailsTextures)
		gl::draw(email.texture, email.position);
}