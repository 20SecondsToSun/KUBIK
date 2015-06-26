#include "KeyboardSettings.h"

using namespace kubik;
using namespace kubik::config;

KeyboardSettings::KeyboardSettings(ApplicationModelRef model):ISettings(model)
{
	
}

void KeyboardSettings::load()
{
	setTextures();
}

void KeyboardSettings::setTextures()
{
	clearResources();
	addToDictionary("backspaceBtnTex", createImageResource(getKeyboardPath("backBtn.png")));
	addToDictionary("_simple", createImageResource(getKeyboardPath("_simpleBtn.png")));
	addToDictionary("_simple1", createImageResource(getKeyboardPath("_simpleBtn1.png")));
	addToDictionary("send", createImageResource(getKeyboardPath("send.png")));
	addToDictionary("search", createImageResource(getKeyboardPath("search.png")));
	addToDictionary("shift", createImageResource(getKeyboardPath("shift.png")));
	addToDictionary("shift0", createImageResource(getKeyboardPath("shift0.png")));
	addToDictionary("space", createImageResource(getKeyboardPath("space.png")));
	addToDictionary("smallSpace", createImageResource(getKeyboardPath("smallSpace.png")));
	addToDictionary("k2", createImageResource(getKeyboardPath("k2.png")));
	addToDictionary("lang", createImageResource(getKeyboardPath("lang.png")));
	addToDictionary("erase", createImageResource(getKeyboardPath("erase.png")));
}

changeSetting::id KeyboardSettings::getChangeID() const
{
	return changeSetting::id::KEYBOARD;
}

std::string KeyboardSettings::getKeyboardPath(const string& value)
{
	ci::fs::path path = getBasePath() / "kubik\\design_elements\\keyboard\\";
	return path.string() + value;
}