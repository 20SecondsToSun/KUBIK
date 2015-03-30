#pragma once

#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"
#include "KeyBoardButtonSprite.h"
#include "TextTools.h"

namespace kubik
{
	class VirtualKeyboard : public Sprite
	{
	public:
		static const int INPUT_TOUCH = 1;
		static const int KEY_TOUCH = 2;
		static const int MAX_LETTER_LIMIT = 3;	
		static const int HIDED = 4;	
		static bool setuped;
		static bool connected;
		static bool showInputField;	

		static VirtualKeyboard& getInstance() { static VirtualKeyboard vk; return vk; };

		void setup();
		void show(ci::Vec2f from, ci::Vec2f to, float time);
		void hide(ci::Vec2f to, float time);
		void hide();
		void draw();
		void update();
		void connectKeyboard();
		void disconnectKeyboard();
		void setPosition(const Vec2f& vec);
		std::string getLastCode();
		std::string getDisplayCode();
		void setInputFieldVisible(bool value);

		static ci::app::KeyEvent   imitate_ENTER_KEY_EVENT();
		static ci::app::KeyEvent   imitate_BACKSPACE_KEY_EVENT();
		static ci::app::MouseEvent inititateMouseEvent(ci::Vec2f _vec);

		bool isMailCode();
		bool isBackCode();
		bool isSendCode();
		bool isShiftCode();
		void checkCapsLock();

		void MouseDown( ci::app::MouseEvent &event );
		void MouseUp( ci::app::MouseEvent &event );
		void alwaysCapsLock(bool value );
		bool inputFieldEmpty();
		void setEraseButtonVisible(bool value);

	private :	
		std::vector<KeyBoardButtonSpriteRef> buttonsMainKeyboard, buttonsSecondKeyboard, buttonsRusMainKeyboard, *activeKeyboard;
		std::vector<KeyBoardButtonSpriteRef> systemKeyboard;

		static std::string secondLineCharacters[10];
		static std::string thirdLineCharacters[9];
		static std::string fourthLineCharacters[9];

		static std::string secondLineCharacters2[10];
		static std::string thirdLineCharacters2[9];
		static std::string fourthLineCharacters2[10];

		static std::string secondLineCharacters3[12];
		static std::string thirdLineCharacters3[11];
		static std::string fourthLineCharacters3[10];

		static ci::Vec2f lineOffset1,lineOffset2,lineOffset3,lineOffset4, lineOffset5;
		static float _xOffset1,_xOffset2,_xOffset3,_xOffset4, _xOffset5;
		static int lineLength1, lineLength2, lineLength3, lineLength4;

		std::string lastCode;
		std::string keyboardMode;
		bool isShiftDown, isKeyBoardChangeDown, showEraseButton;

		ci::gl::Texture	 shiftTex1, shiftTex0;
		ci::gl::Texture	 changeKeyboardTex1, changeKeyboardTex2;
		KeyBoardButtonSpriteRef shift, erase;
		KeyBoardButtonSpriteRef changeKeyboardBtn, changeKeyboardBtnDuplicat;	
		ci::signals::connection KeyDownCon, MouseDownCon, MouseUpCon;
		ci::Anim<ci::Vec2f> keyBoardPosition;

		enum KEYBOARD_LANG {ENG, RUS} activeLanguage;

		void KeyDown(ci::app::KeyEvent event);
		void changeShiftMode();
		void changeKeyboardMode();
		void closeKeyboard();
		void changeLangMode();
		void offCapsLock();
		void onCapsLock();	
		bool alwaysCaps;
		void setLanguage(KEYBOARD_LANG eng);

		SimpleSpriteButtonRef touchInputZone;
		std::string inputField;
		ci::Font inputFont;
		size_t maxInputChars;
		bool isShowing;
		Vec2f touchInputZonePos;
		ci::gl::Texture inputFieldTexture;

		ci::Color inputColor;

		void createInputField()
		{
			originPoint = Vec2f::zero();
			maxInputChars = 20;
			setInputColor(Color::black());
			setInputFont(Font( loadFile(getAssetPath("fonts/IntroLight.ttf")), 44 ));
			setInputFieldText("test");

			touchInputZone  = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0,0, 600, 50)));
			touchInputZone->setColor(Color(1,0,0));
		}

		ci::Vec2f originPoint;

	public:
		void setOriginPoint(Vec2f point)
		{
			originPoint = point;
		}

		void setInputField(float x, float y, float width, float height)
		{
			touchInputZonePos = Vec2f(x, y);
			touchInputZone->setButtonArea1(Rectf(x, y, width, height));
			erase->setPosition(touchInputZonePos + Vec2f(touchInputZone->getWidth() - 50, 0.5f*(touchInputZone->getHeight() - 30)));
		}

		void setInputFont(const ci::Font& font)
		{
			inputFont = font;
		}

		void setInputColor(const ci::Color& color)
		{
			inputColor = color;
		}

		void clearInputFieldText()
		{
			setInputFieldText("");
		}

		void setInputFieldText(const std::string& text)
		{
			inputField = text;
			inputFieldTexture = textTools().getTextField(text, &inputFont, inputColor);		
		}

		std::string getInputFieldText()
		{
			return inputField;
		}

		bool emptyInputField()
		{
			return inputField == "";
		}


		
	};

	inline VirtualKeyboard&	touchKeyboard() { return VirtualKeyboard::getInstance();};
}