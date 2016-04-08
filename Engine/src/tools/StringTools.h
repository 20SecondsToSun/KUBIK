#pragma once

namespace kubik
{
	class StringTools
	{
	public:
		static StringTools& getInstance()
		{
			static StringTools strt;
			return strt;
		};

		std::string cp1251_to_utf8(const char *str);
		std::wstring s2ws(const std::string& s);
		ci::ColorA stringToColor(std::string color);
		std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
		std::vector<std::string> split(const std::string &s, char delim);
	};

	inline StringTools&	stringTools() { return StringTools::getInstance(); };
}