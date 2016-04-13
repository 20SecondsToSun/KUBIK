#pragma once

#include <boost/timer.hpp>

class Utils
{
	public:
		static struct tm getCurrentTime();	
		static double getCurrentMilliseconds();
		static std::string duplicateSymbol(std::string str, std::string symb);
		static ci::gl::Texture loadImageFromString(std::string value);	

		static std::string cp1251_to_utf8(const char *str);
		static std::string Utf8_to_cp1251(const char *str);

		static std::wstring s2ws(const std::string& s);
	    static LPWSTR ConvertToLPWSTR( const std::wstring& s );
		static int StringToWString(std::wstring &ws, const std::string &s);
		static std::string twiceSymbol(std::string __string, char __symbol);

		static bool readFlag(std::string path);
		static void writeCrashLog(std::string path);

		static std::string urlDecode(std::string SRC);

		static void textFieldDraw(std::string text, ci::Font* font, ci::Vec2f coords, ci::ColorA color);
		static float map(float value, float istart, float istop, float ostart, float ostop);
		static float clamp(float value, float max, float min);

		static ci::Surface Utils::resizeScreenshot(ci::Surface screenshot, int32_t width, int32_t height);

		static ci::gl::Texture getTextField(std::string text,ci::Font* font,  ci::ColorA color);
		std::string getCorrectSecondsText(int seconds);

		static void drawGraphicsToFBO(ci::gl::Fbo fbo, const std::function<void ()>& graphicsFunc);
		static ci::gl::Texture drawGraphicsToFBO(int width, int height, const std::function<void()>& graphicsFunc);
		static ci::gl::Texture drawGraphicsToFBO(ci::Vec2i size, const std::function<void()>& graphicsFunc);

		static void clearFBO(ci::gl::Fbo fbo);
		static void printVideoMemoryInfo();

		static ci::ColorA colorAlpha(ci::ColorA color, float alpha);

		static bool validate_email(const std::string& a);

		static std::string fix2(const std::string& strtofix);
};
