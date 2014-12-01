#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"
#include <boost/algorithm/string.hpp>
#include "TextureManager.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Graphics
{

public:

	enum gamesTexturesID
	{
		MENU = 5,
		SETTINGS = 4,
		FUNCES = 1,
		PHOTOBOOTH = 2,
		KOTOPOZA = 3
	};

	typedef struct __tex
	{
		std::string path;
		ci::gl::Texture tex;
		bool isLoading;
	} TexObject;

	typedef map<string, TexObject> OneBlockTexDictionary;
	typedef map<gamesTexturesID, OneBlockTexDictionary*> AllTexDictionary;

	static Graphics& getInstance() { static Graphics graph; return graph; };

	Graphics()
	{
		console()<<" graphics init..."<<endl;

		addToDictionary(MENU, "background" , "mainSkinDesign\\bg.jpg");
		addToDictionary(MENU, "background1", "mainSkinDesign\\bg1.png");
		addToDictionary(MENU, "background2", "mainSkinDesign\\title.jpg");		

		allTexDic[MENU] = &menuDesign;

		mainDesignLoadinsCounter = menuDesign.size();




		addToDictionary(SETTINGS, "background" , "mainSkinDesign\\bg.jpg");
		addToDictionary(SETTINGS, "background1", "mainSkinDesign\\bg1.png");
		addToDictionary(SETTINGS, "background2", "mainSkinDesign\\title.jpg");	

		allTexDic[SETTINGS] = &settingsDesign;

		settingsDesignLoadinsCounter = settingsDesign.size();


		loadingStack = 0;
	}

	void loadTextures(int id)
	{	
		loadingStack++;

		switch (id)
		{
			case MENU:
				menuLoadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadSkinTextures, this ));
			break;

			case FUNCES:
				gameTexLoadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadGameTextures, this ));
			break;

			case SETTINGS:
				loadingStack--;
				//gameTexLoadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadGameTextures, this ));
			break;
		}
	}
	
	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(void)>& handler)
	{
		errorHandler = handler;
	}

	void uloadLastGame()
	{

	}

	bool checkAllLoading()
	{
		return loadingStack == 0;
	}

	AllTexDictionary getTextures()
	{
		return allTexDic;
	}

private:

	void addToDictionary(int id, string key, string path)
	{	
		TexObject value = {path, Texture(), false};

		switch (id)
		{
			case MENU:
				menuDesign[key]  = value;
			break;
		}
	}

	void loadSkinTextures()
	{
		for ( auto it = menuDesign.begin(); it != menuDesign.end(); it++)
		{
			if ((*it).second.isLoading == false)
			{
				string url = getFullTexturePath((*it).second.path);

				if(!ph::isTextureLoaded(url))
				{
					(*it).second.tex = ph::fetchTexture(url);
				}
				else if (mainDesignLoadinsCounter > 0)
				{
					(*it).second.tex = ph::fetchTexture(url);
					(*it).second.isLoading = true;

					if (--mainDesignLoadinsCounter == 0)
					{
						--loadingStack;
						completeHandler();
						menuLoadingSignal.disconnect();
					}
				}
			}
		}
	}

	void loadGameTextures()
	{		
		loadingStack--;
		completeHandler();
		gameTexLoadingSignal.disconnect();
	}

	string getFullTexturePath(string url)
	{
		return getAssetPath(url).string();
	}	

	std::function<void()> completeHandler;
	std::function<void()> errorHandler;

	AllTexDictionary allTexDic;
	OneBlockTexDictionary	menuDesign, settingsDesign, gameDesign;

	int mainDesignLoadinsCounter;
	int settingsDesignLoadinsCounter;
	int loadingStack;

	ci::signals::connection menuLoadingSignal, gameTexLoadingSignal;
};

inline Graphics&	graphics() { return Graphics::getInstance(); };