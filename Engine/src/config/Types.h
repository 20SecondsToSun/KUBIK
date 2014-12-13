#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/qtime/QuickTime.h"
#include <stdint.h>

using namespace std;
using namespace ci;
using namespace ci::gl;

namespace kubik
{
	struct GamesInfo
	{
		int id;
		bool isOn, isPurchased;
		string name;
	};

	enum LocationID
	{
		MENU = 10,
		CONTROL = 20,
		SCREENSAVER = 30,
		PRELOADER = 40,
		SERVICEPOPUP = 50,
	};

	enum gameId
	{
		FUNCES = 1,
		PHOTOBOOTH = 2,
		KOTOPOZA = 3
	};

	enum resourceType
	{
		IMAGE,
		VIDEO,
		FONT
	};

	enum loadingType
	{
		RESOURCE,
		ASSET,
		FULL_PATH,
		URL
	};	

	class IResource
	{
	public:
		string path;
		bool isLoading;
		resourceType resourceType;
		loadingType  loadingType;

		virtual Surface getTex( ) = 0;
		virtual qtime::MovieGl getMovie( ) = 0;
		virtual Font getFont( ) = 0;
		virtual float getFontSize( ) = 0;

		virtual void setTex(Surface surf) = 0;
		virtual void setMovie(qtime::MovieGl movie) = 0;
		virtual void setFont(Font font) = 0;
		virtual void setFontSize(float size) = 0;
	};

	typedef map<string, shared_ptr<IResource>> ResourceDictionary;

	class ImageResource: public IResource
	{
	public:
		Surface tex;


		Surface getTex( )
		{
			return tex;
		}

		void setTex(Surface surf)
		{
			tex = surf;
		}

		qtime::MovieGl getMovie(){return qtime::MovieGl();};
		Font getFont(){return Font();};

		void  setMovie(qtime::MovieGl movie){};
		void  setFont(Font font){};
		void  setFontSize(float size){};
		float getFontSize() {return 0;};
	};

	class FontResource: public IResource
	{
	public:
		Font font;		
		float fontSize;

		Surface getTex(){return Surface();};
		qtime::MovieGl getMovie(){ return qtime::MovieGl();};
		Font getFont()
		{
			return font;
		};

		void setTex(Surface surf){};
		void setMovie(qtime::MovieGl movie){};
		void setFont(Font _font)
		{
			font = _font;
		};

		void setFontSize(float size)
		{
			fontSize = size;
		};

		float getFontSize(){ return fontSize;};
	};

	class VideoResource: public IResource
	{
	public:
		qtime::MovieGl movie;

		Surface getTex(){return Surface();};
		Font getFont(){return Font();};
		qtime::MovieGl getMovie()
		{
			return movie;
		};

		void setTex(Surface surf){};
		void setMovie(qtime::MovieGl _movie)
		{
			movie = _movie;
		};

		void setFont(Font _font)
		{

		};
		void setFontSize(float size){};
		float getFontSize(){return 0;};
	};

	typedef struct _changes
	{
		int id;
		bool texReload;
	}
	Changes;
}

template <typename Sig, typename F> void connect_once(Sig& sig, F&& f) 
{		
	if (!sig.num_slots()) 
		sig.connect(std::forward<F>(f));
}