#include "VirtualKeyboard.h"
#include <ctype.h>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace kubik;

string VirtualKeyboard::secondLineCharacters[10]   = {"q","w","e","r","t","y","u","i","o","p"};
string VirtualKeyboard::thirdLineCharacters[9]     = {"a","s","d","f","g","h","j","k","l"};
string VirtualKeyboard::fourthLineCharacters[9]    = {"z","x","c","v","b","n","m",".","@"};

string VirtualKeyboard::secondLineCharacters2[10]  = {"(",")","{","}","#","%","^","*","+","="};
string VirtualKeyboard::thirdLineCharacters2[9]    = {"\\","|","~","<",">","$","&","'","\""};
string VirtualKeyboard::fourthLineCharacters2[10]  = {"_","-","/",":",";","?","!",",",".","@"};

string VirtualKeyboard::secondLineCharacters3[12]  = {"é","ö","ó","ê","å","í","ã","ø","ù","ç","õ","Ú"};
string VirtualKeyboard::thirdLineCharacters3[11]   = {"ô","û","â","à","ï","ð","î","ë","ä","æ", "ý"};
string VirtualKeyboard::fourthLineCharacters3[10]  = {"ÿ","÷","ñ","ì","è","ò","ü","á","þ", "@"};

int VirtualKeyboard::lineLength1 = 10;

float VirtualKeyboard::_xOffset1 = 14.0f;
float VirtualKeyboard::_xOffset2 = 14.0f;
float VirtualKeyboard::_xOffset3 = 13.0f;
float VirtualKeyboard::_xOffset4 = 12.0f;
float VirtualKeyboard::_xOffset5 = 12.0f;

bool VirtualKeyboard::setuped = false;
bool VirtualKeyboard::connected = false;
bool VirtualKeyboard::showInputField = true;

ci::Vec2f VirtualKeyboard::lineOffset1 = Vec2f(360.0f, 30.0f);
ci::Vec2f VirtualKeyboard::lineOffset2 = Vec2f(415.0f, 122.0f);
ci::Vec2f VirtualKeyboard::lineOffset3 = Vec2f(455.0f, 214.0f);
ci::Vec2f VirtualKeyboard::lineOffset4 = Vec2f(510.0f, 306.0f);
ci::Vec2f VirtualKeyboard::lineOffset5 = Vec2f(504.0f, 398.0f);

void VirtualKeyboard::setup()
{ 
	if (setuped) return;

	Font  mFont	= Font( loadFile(getAssetPath("fonts/Helvetica Neue Light.ttf")), 25 );
	
	gl::Texture backspaceBtnTex				= gl::Texture( loadImage( loadAsset(   "keyboard/backBtn.png" )));
	gl::Texture _simple						= gl::Texture( loadImage( loadAsset(   "keyboard/_simpleBtn.png" )));	
	gl::Texture _simple1					= gl::Texture( loadImage( loadAsset(   "keyboard/_simpleBtn1.png" )));	
	gl::Texture	yaBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/ya.png")));
	gl::Texture ramBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/ram.png")));
	gl::Texture mailBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/mail.png" )));
	gl::Texture gmailBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/gmail.png" )));

	gl::Texture sendBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/send.png" )));
	shiftTex1								= gl::Texture( loadImage( loadAsset(   "keyboard/shift.png" )));
	shiftTex0								= gl::Texture( loadImage( loadAsset(   "keyboard/shift0.png")));

	gl::Texture spaceBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/space.png" )));

	changeKeyboardTex1						= gl::Texture( loadImage( loadAsset(   "keyboard/k2.png" )));
	changeKeyboardTex2						= gl::Texture( loadImage( loadAsset(   "keyboard/k2.png")));

	gl::Texture langChangeTex				= gl::Texture( loadImage( loadAsset(   "keyboard/lang.png")));

	gl::Texture eraseTex					= gl::Texture( loadImage( loadAsset(   "keyboard/erase.png")));

	Vec2f shift_Y = Vec2f::zero();
	float _width  = 79.0f;
	float _width1 = 68.0f;	

	lineOffset1 = Vec2f(0.0f,      0.0f);
	lineOffset2 = Vec2f(65.0f,    88.0f);
	lineOffset3 = Vec2f(105.0f, 2*88.0f);
	lineOffset4 = Vec2f(150.0f, 3*88.0f);
	lineOffset5 = Vec2f(194.0f, 4*88.0f);
	
	for (auto i = 0; i < lineLength1; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, to_string(i)));
		btn->setPosition(lineOffset1 + Vec2f(i*(_xOffset1 + _width), 0.0f)+shift_Y);		
		buttonsMainKeyboard.push_back( btn );
		buttonsSecondKeyboard.push_back( btn );
		buttonsRusMainKeyboard.push_back( btn );
	}

	erase = KeyBoardButtonSpriteRef( new KeyBoardButtonSprite(eraseTex, "erase"));	

	
	shift = KeyBoardButtonSpriteRef( new KeyBoardButtonSprite(shiftTex0, "shift"));
	shift->setPosition(lineOffset4 - Vec2f(91.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(shift);	
	buttonsRusMainKeyboard.push_back(shift);

	KeyBoardButtonSpriteRef backspaceBtn = KeyBoardButtonSpriteRef( new KeyBoardButtonSprite(backspaceBtnTex, "back"));
	backspaceBtn->setPosition(lineOffset1 + Vec2f(10.0f*(_xOffset1 + _width),0.0f)+shift_Y);	
	buttonsMainKeyboard.push_back(backspaceBtn);
	buttonsSecondKeyboard.push_back(backspaceBtn);
	buttonsRusMainKeyboard.push_back(backspaceBtn);	

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, secondLineCharacters[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, secondLineCharacters2[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 12; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, secondLineCharacters3[i]));
		btn->setPosition(lineOffset2 + Vec2f(i*(_xOffset2 + _width1) - 47, 0.0f) + shift_Y);		
		buttonsRusMainKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, thirdLineCharacters[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, thirdLineCharacters2[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}
	
	for (size_t i = 0; i < 11; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, thirdLineCharacters3[i]));
		btn->setPosition(lineOffset3 + Vec2f(i*(_xOffset2 + _width1) - 37, 0.0f) + shift_Y);		
		buttonsRusMainKeyboard.push_back( btn );
	}


	for (size_t i = 0; i < 9; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters[i]));
		btn->setPosition(lineOffset4 + Vec2f(i*(_xOffset4 + _width), 0.0f)+ shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters2[i]));
		btn->setPosition(lineOffset4 + Vec2f((i)*(_xOffset4 + _width), 0.0f)+ shift_Y - Vec2f(_xOffset4 + _width, 0.0f));		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0; i < 10; i++)
	{
		KeyBoardButtonSpriteRef btn;
		if(fourthLineCharacters3[i] != "@")
			btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple1, mFont, fourthLineCharacters3[i]));
		else
			btn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(_simple, mFont, fourthLineCharacters3[i]));

		btn->setPosition(lineOffset4 + Vec2f(i*(_xOffset2 + _width1) - 12, 0.0f) + shift_Y);		
		buttonsRusMainKeyboard.push_back( btn );
	}
	
	KeyBoardButtonSpriteRef spaceBtn , sendBtn , langBtn;

	changeKeyboardBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(changeKeyboardTex1, mFont, "#+="));
	changeKeyboardBtn->setPosition(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width) - 170.0f, 0.0f)+ shift_Y);	
	buttonsMainKeyboard.push_back(changeKeyboardBtn);
	buttonsSecondKeyboard.push_back(changeKeyboardBtn);
	buttonsRusMainKeyboard.push_back(changeKeyboardBtn);

	langBtn = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(langChangeTex, "lang"));
	langBtn->setPosition(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width) - 31.0f, 0.0f) + shift_Y);	
	buttonsMainKeyboard.push_back(langBtn);
	buttonsSecondKeyboard.push_back(langBtn);
	buttonsRusMainKeyboard.push_back(langBtn);

	spaceBtn= KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(spaceBtnTex, " "));
	spaceBtn->setPosition(lineOffset5 + 2.0f*Vec2f((_xOffset5 + _width) - 38, 0.0f)+ shift_Y);	
	buttonsMainKeyboard.push_back(spaceBtn);
	buttonsSecondKeyboard.push_back(spaceBtn);
	buttonsRusMainKeyboard.push_back(spaceBtn);

	changeKeyboardBtnDuplicat = KeyBoardButtonSpriteRef(new KeyBoardButtonSprite(changeKeyboardTex1, mFont, "#+="));
	Vec2f pos = lineOffset5 + 2.0f*Vec2f((_xOffset5  + _width - 38), 0.0f) + shift_Y;
	changeKeyboardBtnDuplicat->setPosition(pos+ Vec2f(_xOffset5 + spaceBtnTex.getWidth(), 0.0f));	

	buttonsMainKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsSecondKeyboard.push_back(changeKeyboardBtnDuplicat);
	buttonsRusMainKeyboard.push_back(changeKeyboardBtnDuplicat);

	alwaysCaps = false;
	isShowing = false;	
	showEraseButton = true;
	setlocale(LC_ALL, ""); 
	createInputField();
	setuped = true;
}

void VirtualKeyboard::show(Vec2f from, Vec2f to, float time)
{
	if(!isShowing)
	{
		erase->setAlpha(1);
		isShowing = true;
		isShiftDown = false;
		isKeyBoardChangeDown = false;
		lastCode = "NONE";
		setLanguage(KEYBOARD_LANG::ENG);

		timeline().apply( &keyBoardPosition, from, to, time, ci::EaseOutCubic());	
	}	
}

void VirtualKeyboard::hide(Vec2f to, float time)
{
	if(isShowing)
	{
		erase->setAlpha(0);
		timeline().apply( &keyBoardPosition, to, time, ci::EaseOutCubic()).finishFn( [ & ]()
		{
			isShowing = false;
			showInputField = true;
		});
	}
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

		default:
			break;
	}
}

void VirtualKeyboard::connectKeyboard()
{	
	if (connected)
		return;
	MouseDownCon   =  getWindow()->getSignalMouseDown().connect(std::bind( &VirtualKeyboard::MouseDown, this, std::placeholders::_1));
	MouseUpCon	   =  getWindow()->getSignalMouseUp().connect(std::bind( &VirtualKeyboard::MouseUp, this, std::placeholders::_1));
	connected = true;
}

void VirtualKeyboard::disconnectKeyboard()
{	
	if (!connected)
		return;

	MouseUpCon.disconnect( );
	MouseDownCon.disconnect();
	connected = false;
}

void VirtualKeyboard::setInputFieldVisible(bool value)
{
	showInputField = value;
}

void VirtualKeyboard::draw( )
{
	if(isShowing)
	{
		gl::pushMatrices();				
			gl::translate(getGlobalPosition());
			gl::translate(keyBoardPosition);
			for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )		
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
			Vec2f(15, 0.5f * (touchInputZone->getHeight() - inputFieldTexture.getHeight()) - 5));
		gl::popMatrices();
		gl::popMatrices();
	}	
}

void VirtualKeyboard::setEraseButtonVisible(bool value)
{
	showEraseButton = value;
}

bool VirtualKeyboard::inputFieldEmpty()
{
	return inputField.empty();
}

void VirtualKeyboard::MouseUp(MouseEvent &event)
{
	if (lastCode == "shift")
	{
		//keyboardTouchSignal();
	}
	else if (lastCode == "erase" && showEraseButton && isShowing)
	{
		inputField = "";
		setInputFieldText(inputField);
	}
	else if (lastCode == "#+=" || lastCode == "ABC")
	{
					
	}
	else if (lastCode == "lang")
	{				
	
	}
	else if (lastCode == "back")
	{				
		if(!inputField.empty())
		{
			inputField.pop_back();
			setInputFieldText(inputField);
		}
	}
	else if (lastCode.size() == 1)
	{				
		callback(KEY_TOUCH);

		if(inputField.size() < maxInputChars)
		{
			inputField += lastCode;
			setInputFieldText(inputField);
		}
		else
		{
			callback(MAX_LETTER_LIMIT);
		}
	}

	
	ci::Vec2f coords = event.getPos() - originPoint;
	
	if (touchInputZone->inButtonField(coords))		
			callback(INPUT_TOUCH);	
}

void VirtualKeyboard::setPosition(const Vec2f& vec)
{
	keyBoardPosition = vec;
}

void VirtualKeyboard::alwaysCapsLock(bool value)
{
	alwaysCaps = value;
}

void VirtualKeyboard::MouseDown( MouseEvent &event )
{
	lastCode = "-1";
	
	if(!isShowing)
		return;

	ci::Vec2f coords = event.getPos() - keyBoardPosition.value() - originPoint;//position.value();

	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{		
		if((*item)->inButtonField(coords))
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

	coords = event.getPos() - originPoint;
	
	if(erase->inButtonField(coords))
	{
		lastCode = erase->getBtnId();
	}	
}

string VirtualKeyboard::getLastCode()
{
	return  lastCode;
}

string VirtualKeyboard::getDisplayCode()
{
	if(alwaysCaps && lastCode.size() == 1)
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
	
	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{			
		if((*item)->getBtnId().size()!=1) continue;
		char letter = (*item)->getBtnId()[0];
		if (isalpha((unsigned char)letter) )
		{		
			string oneChar = "";
			if (isShiftDown)			
				oneChar.append(1, toupper(letter));				
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
	default:
		break;
	}

	setLanguage(activeLanguage);

	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{			
		if((*item)->getBtnId().size()!=1) continue;
		char letter = (*item)->getBtnId()[0];
		if (isalpha((unsigned char)letter) )
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

void VirtualKeyboard::changeKeyboardMode()
{
	isKeyBoardChangeDown  = !isKeyBoardChangeDown;

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

MouseEvent VirtualKeyboard::inititateMouseEvent(ci::Vec2f vec)
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
	if(bCapsIsOn != isShiftDown )//|| alwaysCaps !=bCapsIsOn)
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
	if(!bCapsIsOn)
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
	if(bCapsIsOn)
	{
		keybd_event(VK_CAPITAL, 0x3a, 0, 0);	// these two-line code
		keybd_event(VK_CAPITAL, 0x3a, KEYEVENTF_KEYUP, 0);	// will simulate pressing capslock key
	}
}