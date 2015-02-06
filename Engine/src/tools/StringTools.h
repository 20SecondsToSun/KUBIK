#pragma once

using namespace ci;

class StringTools
{
public:

	static StringTools& getInstance() { 
		static StringTools strt; 
		return strt; 
	};

	string cp1251_to_utf8(const char *str)
	{
		string res;	
		int result_u, result_c;


		result_u = MultiByteToWideChar(1251,	0,	str,-1,	0,	0);

		if (!result_u)
			return 0;

		wchar_t *ures = new wchar_t[result_u];

		if(!MultiByteToWideChar(1251,	0,	str,	-1,	ures,	result_u))
		{
			delete[] ures;
			return 0;
		}

		result_c = WideCharToMultiByte(	CP_UTF8, 0,	ures,	-1,	0,	0,	0, 0);

		if(!result_c)
		{
			delete [] ures;
			return 0;
		}

		char *cres = new char[result_c];

		if(!WideCharToMultiByte(CP_UTF8,	0,	ures,	-1,	cres,	result_c,	0, 0))
		{
			delete[] cres;
			return 0;
		}
		delete[] ures;
		res.append(cres);
		delete[] cres;
		return res;
	}

	std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	ci::ColorA stringToColor(string color)
	{			
		return ci::ColorA::hex(std::stoi(color, 0, 16));
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
	}
	std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
};

inline StringTools&	stringTools() { return StringTools::getInstance(); };