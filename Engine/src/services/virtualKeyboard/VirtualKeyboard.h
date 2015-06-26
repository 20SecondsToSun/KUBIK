#pragma once

#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"
#include "KeyBoardButtonSprite.h"
#include "TextTools.h"
#include "ISettings.h"
#include "fontStorage/FontStorage.h"

namespace kubik
{
	class VirtualKeyboard : public Sprite
	{
	public:
		static const int INPUT_TOUCH = 1;
		static const int KEY_TOUCH = 2;
		static const int MAX_LETTER_LIMIT = 3;	
		static const int HIDED = 4;
		static const int SEND_TOUCH = 5;
		static const int SEARCH_TOUCH = 6;

		static bool setuped;
		static bool connected;
		static bool showInputField;
		static bool carridgeDrawing;
		static bool carridgePhase;
		
		static VirtualKeyboard& getInstance() { static VirtualKeyboard vk; return vk; };

		void create(config::ISettingsRef config);
		void show(const ci::Vec2f& from, const ci::Vec2f& to, float time);
		void hide(const ci::Vec2f& to, float time);
		void hideQuick(const ci::Vec2f& to);
		
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
		static ci::app::MouseEvent inititateMouseEvent(const ci::Vec2f& vec);

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

		void activateSearchMode();
		void activateSendMode();
		void activateUsualMode();

	private :	
		enum modes
		{
			USUAL_MODE,
			SEARCH_MODE,
			SEND_MODE
		}
		mode;

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
		ci::gl::Texture	 smallspaceBtnTex, spaceBtnTex, sendBtnTex, searchBtnTex;
		ci::gl::Texture	 changeKeyboardTex1, changeKeyboardTex2, eraseTex;

		KeyBoardButtonSpriteRef shift, erase;
		KeyBoardButtonSpriteRef changeKeyboardBtn, changeKeyboardBtnDuplicat;	
		KeyBoardButtonSpriteRef spaceBtn, sendBtn, searchBtn;

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
		void clearCurrentMode();
		void createInputField(const ci::Font& font);

		void drawCarriage();

		SimpleSpriteButtonRef touchInputZone;
		std::string inputField;
		ci::Font inputFont;
		size_t maxInputChars;
		bool isShowing;
		Vec2f touchInputZonePos;
		ci::gl::Texture inputFieldTexture;

		ci::Color inputColor;
		ci::Vec2f originPoint;		

	public:
		void setOriginPoint(const ci::Vec2f& point);
		void setInputField(float x, float y, float width, float height);
		void setInputFont(const ci::Font& font);
		void setInputColor(const ci::Color& color);
		void clearInputFieldText();
		void setDefaultSettings();		
		void setInputFieldText(const std::string& text);
		void setEraseButtonTexture(const gl::Texture& value);
		std::string getInputFieldText() const;
		bool emptyInputField();
		bool showing() const;
	};

	inline VirtualKeyboard&	touchKeyboard() { return VirtualKeyboard::getInstance();};
}