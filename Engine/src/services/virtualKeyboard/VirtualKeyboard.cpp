#include "VirtualKeyboard.h"
#include <ctype.h>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik;

string VirtualKeyboard::secondLineCharacters[10] = { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p" };
string VirtualKeyboard::thirdLineCharacters[9] = { "a", "s", "d", "f", "g", "h", "j", "k", "l" };
string VirtualKeyboard::fourthLineCharacters[9] = { "z", "x", "c", "v", "b", "n", "m", ".", "@" };

string VirtualKeyboard::secondLineCharacters2[10] = { "(", ")", "{", "}", "#", "%", "^", "*", "+", "=" };
string VirtualKeyboard::thirdLineCharacters2[9] = { "\\", "|", "~", "<", ">", "$", "&", "'", "\"" };
string VirtualKeyboard::fourthLineCharacters2[10] = { "_", "-", "/", ":", ";", "?", "!", ",", ".", "@" };

string VirtualKeyboard::secondLineCharacters3[12] = { "é", "ö", "ó", "ê", "å", "í", "ã", "ø", "ù", "ç", "õ", "Ú" };
string VirtualKeyboard::thirdLineCharacters3[11] = { "ô", "û", "â", "à", "ï", "ð", "î", "ë", "ä", "æ", "ý" };
string VirtualKeyboard::fourthLineCharacters3[10] = { "ÿ", "÷", "ñ", "ì", "è", "ò", "ü", "á", "þ", "@" };

int VirtualKeyboard::lineLength1 = 10;

float VirtualKeyboard::_xOffset1 = 14.0f;
float VirtualKeyboard::_xOffset2 = 14.0f;
float VirtualKeyboard::_xOffset3 = 13.0f;
float VirtualKeyboard::_xOffset4 = 12.0f;
float VirtualKeyboard::_xOffset5 = 12.0f;

bool VirtualKeyboard::setuped = false;
bool VirtualKeyboard::connected = false;
bool VirtualKeyboard::showInputField = true;
bool VirtualKeyboard::carridgeDrawing = false;
bool VirtualKeyboard::carridgePhase = false;

ci::Vec2f VirtualKeyboard::lineOffset1 = Vec2f(360.0f, 30.0f);
ci::Vec2f VirtualKeyboard::lineOffset2 = Vec2f(415.0f, 122.0f);
ci::Vec2f VirtualKeyboard::lineOffset3 = Vec2f(455.0f, 214.0f);
ci::Vec2f VirtualKeyboard::lineOffset4 = Vec2f(510.0f, 306.0f);
ci::Vec2f VirtualKeyboard::lineOffset5 = Vec2f(504.0f, 398.0f);

void VirtualKeyboard::create(config::ISettingsRef config)
{
	if (setuped) return;

	Font mFont = getFont("HelveticaLight", 25);
	gl::Texture backspaceBtnTex = config->getTexture("backspaceBtnTex");
	gl::Texture _simple = config->getTexture("_simple");
	gl::Texture _simple1 = config->getTexture("_simple1");

	sendBtnTex = config->getTexture("send");
	searchBtnTex = config->getTexture("search");
	shiftTex1 = config->getTexture("shift");
	shiftTex0 = config->getTexture("shift0");

	spaceBtnTex = config->getTexture("space");
	smallspaceBtnTex = config->getTexture("smallSpace");

	changeKeyboardTex1 = config->getTexture("k2");
	changeKeyboardTex2 = config->getTexture("k2");

	gl::Texture langChangeTex = config->getTexture("lang");

	eraseTex = config->getTexture("erase");

	Vec2f shift_Y = Vec2f::zero();
	float _width = 79.0f;
	float _width1 = 68.0f;

	lineOffset1 = Vec2f::zero();
	lineOffset2 = Vec2f(65.0f, 88.0f);
	lineOffset3 = Vec2f(105.0f, 2 * 88.0f);
	lineOffset4 = Vec2f(150.0f, 3 * 88.0f);
	lineOffset5 = Vec2f(194.0f, 4 * 88.0f);

	for (auto i = 0; i < lineLength1; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, to_string(i)));
		btn->setPosition(lineOffset1 + Vec2f(i*(_xOffset1 + _width), 0.0f) + shift_Y);
		buttonsMainKeyboard.push_back(btn);
		buttonsSecondKeyboard.push_back(btn);
		buttonsRusMainKeyboard.push_back(btn);
	}

	erase = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(eraseTex, "erase"));

	shift = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(shiftTex0, "shift"));
	shift->setPosition(lineOffset4 - Vec2f(91.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(shift);
	buttonsRusMainKeyboard.push_back(shift);

	KeyBoardButtonSpriteRef backspaceBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(backspaceBtnTex, "back"));
	backspaceBtn->setPosition(lineOffset1 + Vec2f(10.0f*(_xOffset1 + _width), 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(backspaceBtn);
	buttonsSecondKeyboard.push_back(backspaceBtn);
	buttonsRusMainKeyboard.push_back(backspaceBtn);

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, secondLineCharacters[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);
		buttonsMainKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, secondLineCharacters2[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);
		buttonsSecondKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 12; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, secondLineCharacters3[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width1) - 47, 0.0f) + shift_Y);
		buttonsRusMainKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, thirdLineCharacters[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);
		buttonsMainKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, thirdLineCharacters2[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);
		buttonsSecondKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 11; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, thirdLineCharacters3[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset2 + _width1) - 37, 0.0f) + shift_Y);
		buttonsRusMainKeyboard.push_back(btn);
	}


	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters[i]));
		btn->setPosition(lineOffset4 + Vec2f(i*(_xOffset4 + _width), 0.0f) + shift_Y);
		buttonsMainKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters2[i]));
		btn->setPosition(lineOffset4 + Vec2f((i)*(_xOffset4 + _width), 0.0f) + shift_Y - Vec2f(_xOffset4 + _width, 0.0f));
		buttonsSecondKeyboard.push_back(btn);
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn;
		if (fourthLineCharacters3[i] != "@")
			btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, fourthLineCharacters3[i]));
		else
			btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters3[i]));

		btn->setPosition(lineOffset4 + Vec2f(i*(_xOffset2 + _width1) - 12, 0.0f) + shift_Y);
		buttonsRusMainKeyboard.push_back(btn);
	}

	KeyBoardButtonSpriteRef langBtn;

	changeKeyboardBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(changeKeyboardTex1, mFont, "#+="));
	changeKeyboardBtn->setPosition(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width) - 170.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(changeKeyboardBtn);
	buttonsSecondKeyboard.push_back(changeKeyboardBtn);
	buttonsRusMainKeyboard.push_back(changeKeyboardBtn);

	langBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(langChangeTex, "lang"));
	langBtn->setPosition(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width) - 31.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(langBtn);
	buttonsSecondKeyboard.push_back(langBtn);
	buttonsRusMainKeyboard.push_back(langBtn);

	spaceBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(spaceBtnTex, " "));
	spaceBtn->setPosition(lineOffset5 + 2.0f*Vec2f((_xOffset5 + _width) - 38, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(spaceBtn);
	buttonsSecondKeyboard.push_back(spaceBtn);
	buttonsRusMainKeyboard.push_back(spaceBtn);

	changeKeyboardBtnDuplicat = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(changeKeyboardTex1, mFont, "#+="));
	Vec2f pos = lineOffset5 + 2.0f*Vec2f((_xOffset5 + _width - 38), 0.0f) + shift_Y;
	changeKeyboardBtnDuplicat->setPosition(pos + Vec2f(_xOffset5 + spaceBtnTex.getWidth(), 0.0f));

	buttonsMainKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsSecondKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsRusMainKeyboard.push_back(changeKeyboardBtnDuplicat);

	searchBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(searchBtnTex, "search"));
	searchBtn->setPosition(pos + Vec2f(_xOffset5 + spaceBtnTex.getWidth() - 31, 0.0f));

	sendBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(sendBtnTex, "send"));
	sendBtn->setPosition(pos + Vec2f(_xOffset5 + spaceBtnTex.getWidth() - 31, 0.0f));

	mode = USUAL_MODE;
	alwaysCaps = false;
	isShowing = false;
	showEraseButton = true;
	setlocale(LC_ALL, "");
	createInputField(getFont("IntroLight", 44));
	setuped = true;
	carridgeDrawing = false;
	carridgePhase = false;
}

void VirtualKeyboard::show(const ci::Vec2f& from, const ci::Vec2f& to, float time)
{
	if (!isShowing)
	{
		erase->setAlpha(1);
		isShowing = true;
		isShiftDown = false;
		isKeyBoardChangeDown = false;
		lastCode = "NONE";
		setLanguage(KEYBOARD_LANG::ENG);

		timeline().apply(&keyBoardPosition, from, to, time, ci::EaseOutCubic());
	}
}

void VirtualKeyboard::hide(const ci::Vec2f& to, float time)
{
	if (isShowing)
	{
		carridgeDrawing = false;
		erase->setAlpha(0);
		timeline().apply(&keyBoardPosition, to, time, ci::EaseOutCubic()).finishFn([&]()
		{
			isShowing = false;
			showInputField = true;
		});
	}
}

void VirtualKeyboard::hideQuick(const ci::Vec2f& value)
{
	keyBoardPosition = value;
	isShowing = false;
}

void VirtualKeyboard::connectKeyboard()
{
	if (!connected)
	{
		MouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&VirtualKeyboard::MouseDown, this, std::placeholders::_1));
		MouseUpCon = getWindow()->getSignalMouseUp().connect(std::bind(&VirtualKeyboard::MouseUp, this, std::placeholders::_1));
		connected = true;
	}
}

void VirtualKeyboard::disconnectKeyboard()
{
	if (connected)
	{
		MouseUpCon.disconnect();
		MouseDownCon.disconnect();
		connected = false;
	}
}

void VirtualKeyboard::draw()
{
	if (isShowing)
	{
		gl::pushMatrices();
		gl::translate(getGlobalPosition());
		gl::translate(keyBoardPosition);
		for (auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item)
			(*item)->draw();
		gl::popMatrices();
	}

	if (showInputField)
	{
		gl::pushMatrices();
		gl::translate(getGlobalPosition());
		gl::pushMatrices();
		//touchInputZone->draw();	
		if (!inputFieldEmpty() && isShowing && showEraseButton)
			erase->draw();

		gl::draw(inputFieldTexture,
			touchInputZone->getLocalPosition() +
			Vec2f(19, 0.5f * (touchInputZone->getHeight() - inputFieldTexture.getHeight())));
		drawCarriage();
		gl::popMatrices();
		gl::popMatrices();
	}
}

void VirtualKeyboard::drawCarriage()
{
	auto position = touchInputZone->getLocalPosition() + Vec2f(19.0f, 0.5f * (touchInputZone->getHeight() - inputFieldTexture.getHeight()));
	auto x1 = position.x + inputFieldTexture.getWidth();
	auto x2 = position.x + inputFieldTexture.getWidth() + 2.0f;
	auto y1 = position.y;
	auto y2 = position.y + inputFieldTexture.getHeight();

	if (carridgeDrawing)
	{
		if (getElapsedFrames() % 30 == 0)
			carridgePhase = !carridgePhase;

		if (carridgePhase)
		{
			gl::color(inputColor);
			gl::drawSolidRect(Rectf(x1, y1, x2, y2));
		}
	}
}

void VirtualKeyboard::MouseUp(MouseEvent &event)
{
	ci::Vec2f coords = event.getPos() - originPoint;
	if (touchInputZone->inButtonField(coords))
	{
		if (isShowing)
			carridgeDrawing = true;
	}

	if (lastCode == "erase" && showEraseButton && isShowing)
	{
		inputField = "";
		setInputFieldText(inputField);
	}
	else if (lastCode == "search")
	{
		callback(SEARCH_TOUCH);
	}
	else if (lastCode == "send")
	{
		callback(SEND_TOUCH);
	}
	else if (lastCode == "back")
	{
		if (!inputField.empty())
		{
			inputField.pop_back();
			setInputFieldText(inputField);
			callback(KEY_TOUCH);
		}
	}
	else if (lastCode.size() == 1)
	{
		callback(KEY_TOUCH);

		if (inputFieldTexture.getWidth() < touchInputZone->getWidth() - 110)
		{
			inputField += lastCode;
			setInputFieldText(inputField);
		}
		else
		{
			callback(MAX_LETTER_LIMIT);
		}
	}
}

void VirtualKeyboard::MouseDown(MouseEvent &event)
{
	lastCode = "-1";

	ci::Vec2f coords = event.getPos() - originPoint;

	if (touchInputZone->inButtonField(coords))
	{
		callback(INPUT_TOUCH);
		if (erase->inButtonField(coords))
			lastCode = "erase";
		return;
	}

	if (!isShowing)
		return;

	coords = event.getPos() - keyBoardPosition.value() - originPoint;

	for (auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item)
	{
		if ((*item)->inButtonField(coords))
		{
			checkCapsLock();
			lastCode = (*item)->getBtnId();
			(*item)->down();

			if (lastCode == "shift")
				changeShiftMode();
			else if (lastCode == "#+=" || lastCode == "ABC")
				changeKeyboardMode();
			else if (lastCode == "lang")
				changeLangMode();
			break;
		}
	}

	if (erase->inButtonField(coords))
		lastCode = erase->getBtnId();
}

void VirtualKeyboard::setLanguage(KEYBOARD_LANG activeLanguage)
{
	switch (activeLanguage)
	{
	case KEYBOARD_LANG::ENG:
		activeKeyboard = &buttonsMainKeyboard;
		shift->setPosition(lineOffset4 - Vec2f(91.0f, 0.0f));
		break;

	case KEYBOARD_LANG::RUS:
		activeKeyboard = &buttonsRusMainKeyboard;
		shift->setPosition(lineOffset4 - Vec2f(91.0f + 14, 0.0f));
		break;
	}
}

void VirtualKeyboard::createInputField(const ci::Font& font)
{
	originPoint = Vec2f::zero();
	maxInputChars = 20;
	setInputColor(Color::black());
	setInputFont(font);
	setInputFieldText("test");

	touchInputZone = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0, 0, 600, 50)));
	touchInputZone->setColor(Color(1, 0, 0));
}

void VirtualKeyboard::setOriginPoint(const Vec2f& point)
{
	originPoint = point;
}

void VirtualKeyboard::setInputField(float x, float y, float width, float height)
{
	touchInputZonePos = Vec2f(x, y);
	touchInputZone->setButtonArea1(Rectf(x, y, width, height));
	erase->setPosition(touchInputZonePos + Vec2f(-39.0f + touchInputZone->getWidth() - erase->getWidth(), 0.5f * (touchInputZone->getHeight() - erase->getHeight())));
}

void VirtualKeyboard::setInputFont(const ci::Font& font)
{
	inputFont = font;
}

void VirtualKeyboard::setInputColor(const ci::Color& color)
{
	inputColor = color;
}

void VirtualKeyboard::clearInputFieldText()
{
	setInputFieldText("");
}

void VirtualKeyboard::setEraseButtonTexture(const gl::Texture& value)
{
	erase->changeTexture(value);
}

void VirtualKeyboard::setDefaultSettings()
{
	erase->changeTexture(eraseTex);
}

void VirtualKeyboard::setInputFieldText(const std::string& text)
{
	inputField = text;
	inputFieldTexture = textTools().getTextField(text, &inputFont, inputColor);
}

std::string VirtualKeyboard::getInputFieldText() const
{
	return inputField;
}

bool VirtualKeyboard::emptyInputField()
{
	return inputField == "";
}

bool VirtualKeyboard::showing() const
{
	return isShowing;
}

void VirtualKeyboard::setInputFieldVisible(bool value)
{
	showInputField = value;
}

void VirtualKeyboard::setEraseButtonVisible(bool value)
{
	showEraseButton = value;
}

bool VirtualKeyboard::inputFieldEmpty()
{
	return inputField.empty();
}

void VirtualKeyboard::setPosition(const Vec2f& vec)
{
	keyBoardPosition = vec;
}

void VirtualKeyboard::alwaysCapsLock(bool value)
{
	alwaysCaps = value;
}

string VirtualKeyboard::getLastCode()
{
	return  lastCode;
}

string VirtualKeyboard::getDisplayCode()
{
	if (alwaysCaps && lastCode.size() == 1)
	{
		char letter = lastCode[0];
		letter = toupper(letter);
		lastCode[0] = letter;
	}

	return Utils::cp1251_to_utf8(lastCode.c_str());
}

void VirtualKeyboard::changeShiftMode()
{
	isShiftDown = !isShiftDown;
	if (isShiftDown)
		shift->changeTexture(shiftTex1);
	else
		shift->changeTexture(shiftTex0);

	for (auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item)
	{
		if ((*item)->getBtnId().size() != 1) continue;
		char letter = (*item)->getBtnId()[0];
		if (letter == '÷' || letter == '×')
		{
			string oneChar = "";
			if (isShiftDown)
			{
				if (letter == '÷')
				{
					oneChar.append(1, '×');
				}
			}
			else if (letter == '×')
			{
				oneChar.append(1, '÷');
			}
			(*item)->setBtnId(oneChar);
		}
		//if (isalpha(letter, loc1))
		if (isalpha((unsigned char)letter))
		{

			string oneChar = "";
			if (isShiftDown)
			{
				if (letter == 'ÿ')
					oneChar.append(1, 'ß');
				else if (letter == '÷')
				{
					oneChar.append(1, '×');

				}
				else
					oneChar.append(1, toupper(letter));
			}
			else
				if (letter == 'ß')
					oneChar.append(1, 'ÿ');
				else if (letter == '×')
					oneChar.append(1, '÷');
				else
					oneChar.append(1, tolower(letter));

				(*item)->setBtnId(oneChar);
		}
	}
	checkCapsLock();
}

void VirtualKeyboard::changeLangMode()
{
	switch (activeLanguage)
	{
	case KEYBOARD_LANG::ENG:
		activeLanguage = KEYBOARD_LANG::RUS;
		break;

	case KEYBOARD_LANG::RUS:
		activeLanguage = KEYBOARD_LANG::ENG;
		break;
	}

	setLanguage(activeLanguage);

	for (auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item)
	{
		if ((*item)->getBtnId().size() != 1) continue;
		char letter = (*item)->getBtnId()[0];

		if (isalpha((unsigned char)letter))
		{
			std::string oneChar = "";

			if (isShiftDown)
				oneChar.append(1, toupper(letter));
			else
				oneChar.append(1, tolower(letter));

			(*item)->setBtnId(oneChar);
		}
	}
}

void VirtualKeyboard::activateSearchMode()
{
	if (mode == SEARCH_MODE)
		return;

	clearCurrentMode();
	spaceBtn->changeTexture(smallspaceBtnTex);

	buttonsMainKeyboard.push_back(searchBtn);
	buttonsSecondKeyboard.push_back(searchBtn);
	buttonsRusMainKeyboard.push_back(searchBtn);

	mode = SEARCH_MODE;
}

void VirtualKeyboard::activateSendMode()
{
	if (mode == SEND_MODE)
		return;

	clearCurrentMode();
	spaceBtn->changeTexture(smallspaceBtnTex);

	buttonsMainKeyboard.push_back(sendBtn);
	buttonsSecondKeyboard.push_back(sendBtn);
	buttonsRusMainKeyboard.push_back(sendBtn);

	mode = SEND_MODE;
}

void VirtualKeyboard::activateUsualMode()
{
	if (mode == USUAL_MODE)
		return;

	clearCurrentMode();
	spaceBtn->changeTexture(spaceBtnTex);

	buttonsMainKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsSecondKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsRusMainKeyboard.push_back(changeKeyboardBtnDuplicat);

	mode = USUAL_MODE;
}

void VirtualKeyboard::clearCurrentMode()
{
	KeyBoardButtonSpriteRef deleteBtn;
	switch (mode)
	{
	case USUAL_MODE:
		deleteBtn = changeKeyboardBtnDuplicat;
		break;

	case SEND_MODE:
		deleteBtn = sendBtn;
		break;

	case SEARCH_MODE:
		deleteBtn = searchBtn;
		break;
	}

	buttonsMainKeyboard.erase(std::remove(buttonsMainKeyboard.begin(), buttonsMainKeyboard.end(), deleteBtn), buttonsMainKeyboard.end());
	buttonsSecondKeyboard.erase(std::remove(buttonsSecondKeyboard.begin(), buttonsSecondKeyboard.end(), deleteBtn), buttonsSecondKeyboard.end());
	buttonsRusMainKeyboard.erase(std::remove(buttonsRusMainKeyboard.begin(), buttonsRusMainKeyboard.end(), deleteBtn), buttonsRusMainKeyboard.end());
}

void VirtualKeyboard::changeKeyboardMode()
{
	isKeyBoardChangeDown = !isKeyBoardChangeDown;

	if (isKeyBoardChangeDown)
	{
		changeKeyboardBtn->setBtnId("ABC");
		changeKeyboardBtnDuplicat->setBtnId("ABC");
		activeKeyboard = &buttonsSecondKeyboard;
	}
	else
	{
		changeKeyboardBtn->setBtnId("#+=");
		changeKeyboardBtnDuplicat->setBtnId("#+=");
		setLanguage(activeLanguage);
	}
}

KeyEvent VirtualKeyboard::imitate_ENTER_KEY_EVENT()
{
	KeyEvent key(getWindow(), 13, 13, 13, 0, 13);
	return key;
}

KeyEvent VirtualKeyboard::imitate_BACKSPACE_KEY_EVENT()
{
	KeyEvent key(getWindow(), 8, 8, ci::app::KeyEvent::KEY_BACKSPACE, 0, 8);
	return key;
}

MouseEvent VirtualKeyboard::inititateMouseEvent(const ci::Vec2f& vec)
{
	unsigned int k = 1;
	float r = 1.0f;
	uint32_t t = 1;
	return MouseEvent(getWindow(), 1, vec.x, vec.y, k, r, t);
}

bool VirtualKeyboard::isMailCode()
{
	return lastCode == "gmail.com" || lastCode == "mail.ru" || lastCode == "yandex.ru" || lastCode == "rambler.ru";
}

bool VirtualKeyboard::isBackCode()
{
	return lastCode == "back";
}

bool VirtualKeyboard::isSendCode()
{
	return lastCode == "send";
}

bool VirtualKeyboard::isShiftCode()
{
	return lastCode == "shift";
}

void VirtualKeyboard::checkCapsLock()
{
	BYTE kbd[256];
	GetKeyboardState(kbd);
	BOOL bCapsIsOn = kbd[VK_CAPITAL];
	//console()<<"alwaysCaps  "<<alwaysCaps<<"  "<<bCapsIsOn<<endl;
	if (bCapsIsOn != isShiftDown)//|| alwaysCaps !=bCapsIsOn)
	{
		keybd_event(VK_CAPITAL, 0x3a, 0, 0);	// these two-line code
		keybd_event(VK_CAPITAL, 0x3a, KEYEVENTF_KEYUP, 0);	// will simulate pressing capslock key
	}
}

void VirtualKeyboard::onCapsLock()
{
	BYTE kbd[256];
	GetKeyboardState(kbd);
	BOOL bCapsIsOn = kbd[VK_CAPITAL];
	if (!bCapsIsOn)
	{
		keybd_event(VK_CAPITAL, 0x3a, 0, 0);	// these two-line code
		keybd_event(VK_CAPITAL, 0x3a, KEYEVENTF_KEYUP, 0);	// will simulate pressing capslock key
	}
}

void VirtualKeyboard::offCapsLock()
{
	BYTE kbd[256];
	GetKeyboardState(kbd);
	BOOL bCapsIsOn = kbd[VK_CAPITAL];
	if (bCapsIsOn)
	{
		keybd_event(VK_CAPITAL, 0x3a, 0, 0);	// these two-line code
		keybd_event(VK_CAPITAL, 0x3a, KEYEVENTF_KEYUP, 0);	// will simulate pressing capslock key
	}
}