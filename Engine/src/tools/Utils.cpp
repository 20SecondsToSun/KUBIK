#include "Utils.h"
#include <regex>

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

struct tm Utils::getCurrentTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	//char       buf[80];
	tstruct = *localtime(&now);
	
	return tstruct;
}

double Utils::getCurrentMilliseconds()
{
	boost::timer t;	
	return t.elapsed();
}

string Utils::fix2(const string& strtofix)
{
	if (strtofix.size() <= 1)
	{
		return "0" + strtofix;
	}
	return "" + strtofix;
}

std::string  Utils::duplicateSymbol(std::string str, std::string symb)
{
	return "";
}

gl::Texture Utils::loadImageFromString(std::string value)
{	
	Buffer buff = fromBase64(value);
	auto myImage = ci::loadImage(DataSourceBuffer::create(buff), ImageSource::Options(), "png");
	return gl::Texture(myImage);
}

string Utils::cp1251_to_utf8(const char *str)
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

string Utils::Utf8_to_cp1251(const char *str)
{
	string res;	
	int result_u, result_c;


	result_u = MultiByteToWideChar(CP_UTF8,	0,	str, -1, 0,	0);	
	if (!result_u)	return 0;

	wchar_t *ures = new wchar_t[result_u];

	if(!MultiByteToWideChar(CP_UTF8, 0,	str, -1, ures,	result_u))
	{
		delete[] ures;
		return 0;
	}


	result_c = WideCharToMultiByte(	1251,	0,	ures, -1,	0,	0,	0, 0);

	if(!result_c)
	{
		delete [] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if(!WideCharToMultiByte(1251,	0,	ures,	-1,	cres,	result_c,	0, 0))
	{
		delete[] cres;
		return 0;
	}

	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}


int Utils::StringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());
	ws = wsTmp;
	return 0;
}

LPWSTR Utils::ConvertToLPWSTR( const std::wstring& s )
{
	LPWSTR ws = new wchar_t[s.size()+1];
	copy( s.begin(), s.end(), ws );
	ws[s.size()] = 0;
	return ws;
}

std::wstring Utils::s2ws(const std::string& s)
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

std::string Utils::twiceSymbol(std::string __string, char __symbol)
{
	string twiceString = "";
	for (size_t  i = 0; i < __string.size(); i++)
	{
		twiceString.push_back( __string[i]);
		if (__string[i] == __symbol ) twiceString.push_back( __string[i]);
	}
	return twiceString;
}

bool Utils::readFlag(std::string path)
{
	string line;
	ifstream myfile;		
	vector<string> cam;
	myfile.open( getAssetPath(path).c_str(), ifstream::in);

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,line);
			cam.push_back(line);						
		}
		myfile.close();	

		return ::atoi(cam[0].c_str()) == 1;
	}

	return false;
}


std::string Utils::urlDecode(std::string SRC) 
{
	std::string ret;
	char ch;
	size_t  i, ii;
	for ( i=0; i<SRC.length(); i++) 
	{
		if (int(SRC[i])==37)
		{
			sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
			ch=static_cast<char>(ii);
			ret+=ch;
			i=i+2;
		} 
		else
		{
			ret+=SRC[i];
		}
	}
	return (ret);
}

void Utils::writeCrashLog(std::string path)
{
	string line;
	ifstream myfile;		
	vector<string> cam;
	myfile.open( getAssetPath(path).c_str(), ifstream::in);

	int value = 0;

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,line);
			cam.push_back(line);						
		}
		myfile.close();	

		value = ::atoi(cam[0].c_str());
	}

	value++;

	ofstream myfile1 (getAssetPath(path).c_str());
	if (myfile1.is_open())
	{
		myfile1 << value;		
		myfile.close();
	}
}

void Utils::textFieldDraw(std::string text,ci::Font *font, Vec2f coords, ColorA color)
{
	gl::pushMatrices();
	gl::translate(coords);
	ci::TextLayout simple;
	//simple.clear(ColorA(1,1,1,0));
	simple.setFont( *font );
	simple.setColor(color );
	simple.addLine(cp1251_to_utf8(text.c_str()));	
	gl::Texture render = gl::Texture( simple.render( true, false ) );
	gl::draw(render);
	gl::popMatrices();
	gl::color(ColorA(1, 1, 1, 1));


	/*
	// create a text box (rectangular text area)
	TextBox mTextBox = TextBox(400,500);
	// set font and font size
	mTextBox.setFont( fonts().getFont("Helvetica Neue Bold") );
	mTextBox.setFontSize( 14.0f );
	// break lines between words
	mTextBox.setBoundary( Text::WORD );
	// adjust space between lines
	mTextBox.setLineSpace( 1.5f );

	// load a text and hand it to the text box
	mTextBox.setText("ffffffffffff");// cp1251_to_utf8(text.c_str()) );

	//console()<<"ddddd"<<endl;

	mTextBox.draw();*/

}

gl::Texture  Utils::getTextField(std::string text,ci::Font* font, ColorA color)
{		
	ci::TextLayout simple;
	simple.clear(ColorA(1, 1, 1, 0));
	simple.setFont(*font);
	simple.setColor(color);
	simple.addLine(cp1251_to_utf8(text.c_str()));			
	return gl::Texture(simple.render(true, false));
}

float Utils::map(float value, float istart, float istop, float ostart, float ostop) 
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

float Utils::clamp(float value, float max, float min)
{
	if (value > max)
	{
		value = max;
	}
	else if (value < min)
	{
		value = min;
	}

	return value;
}

Surface Utils::resizeScreenshot(Surface screenshot, int32_t width, int32_t height)
{
	Surface screenshot_r = Surface( width, height, false ); 
	ci::ip::resize( screenshot, screenshot.getBounds(), &screenshot_r, screenshot_r.getBounds(), FilterBox() );

	return screenshot_r;
}

std::string  Utils::getCorrectSecondsText(int seconds)
{
	string secText  = to_string(seconds);
	string memo = "������";

	if(secText == "1")
	{
		memo = "�������";
	}
	else if(secText == "2" || secText == "3"|| secText == "4")
	{
		memo = "�������";
	}
	else if(secText.size() == 2)
	{
		if(secText[0] == '1')
		{
			memo = "������";
		}
		else
		{
			if(secText[1] == '1')
			{
				memo = "�������";
			}
			else if(secText[1] == '0' || secText[1] >= '5')
			{
				memo = "������";
			}
			else
			{
				memo = "�������";
			}
		}				
	}	

	return memo;
}

void  Utils::drawGraphicsToFBO(Fbo fbo, const function<void ()>& graphicsFunc )
{
	gl::SaveFramebufferBinding bindingSaver;
	fbo.bindFramebuffer();

	Area saveView = getViewport();
	gl::setViewport(fbo.getBounds());

	gl::pushMatrices();
	gl::setMatricesWindow(fbo.getSize(), false);
	graphicsFunc();
	gl::popMatrices();
	gl::setViewport(saveView);
}

ci::gl::Texture Utils::drawGraphicsToFBO(ci::Vec2i size, const function<void ()>& graphicsFunc )
{
	return drawGraphicsToFBO(size.x, size.y, graphicsFunc);
}

ci::gl::Texture Utils::drawGraphicsToFBO(int width, int height, const function<void ()>& graphicsFunc )
{
	Fbo::Format format = Fbo::Format();
	format.setMagFilter(GL_NEAREST);
	format.enableMipmapping();
	format.enableColorBuffer();
	format.setMinFilter(GL_NEAREST);
	format.setColorInternalFormat(GL_RGBA);		
	Fbo fbo = gl::Fbo(width, height, format);

	gl::SaveFramebufferBinding bindingSaver;
	fbo.bindFramebuffer();

	Area saveView = getViewport();
	gl::setViewport(fbo.getBounds());

	gl::pushMatrices();
	gl::setMatricesWindow(fbo.getSize(), false);
	gl::enableAlphaBlending();
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	graphicsFunc();
	gl::popMatrices();
	gl::setViewport(saveView);

	auto retTex = fbo.getTexture();
	clearFBO(fbo);	

	return retTex;
}

void Utils::clearFBO(Fbo fbo)
{		
	if (!fbo || !fbo.getTexture())
	{
		return;
	}

	GLuint depthTextureId = fbo.getDepthTexture().getId();
	fbo.reset();	
	if (depthTextureId > 0)
	{
		glDeleteTextures(1, &depthTextureId);
	}
}

void Utils::printVideoMemoryInfo()
{
	GLint v[2] = { 0, 0 };
	glGetIntegerv(0x9047, &v[0]);
	glGetIntegerv(0x9049, &v[1]);
	console()<< "Size: " << v[0] << " KB, Available: " << v[1] << " KB" << std::endl;
}

ci::ColorA Utils::colorAlpha(ci::ColorA color, float alpha) 
{
	return ci::ColorA(color.r, color.g, color.b, alpha); 
}

bool Utils::validate_email(const std::string& email)
{
	return email != "" && regex_match(email, std::regex("[a-z0-9�-��-�._%+-]+@[a-z0-9.-]+\\.[a-z�-�]{2,10}"));
}


ci::Vec2f Utils::transformCoords(const ci::Vec2f& coords)
{

	auto _size = Vec2f(1080, 1920);
	auto coord = coords;

#ifdef release

	////coord.x = coords.y;
	////coord.y = _size.x - coords.x;

	////coord.x = ((double)coord.x / _size.y) * _size.x;
	////coord.y = ((double)coord.y / _size.x) * _size.y;

	
	coord.x = _size.x - coord.x;
	coord.y = _size.y - coord.y;

#endif
	coord.x = _size.x - coord.x;
	coord.y = _size.y - coord.y;

	return coord;
}