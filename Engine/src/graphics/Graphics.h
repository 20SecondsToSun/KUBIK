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
		SKIN_1,
		SKIN_2,
		GAME_1,
		GAME_2,
	};

	typedef struct __tex
	{
		string path;
		Texture tex;
		bool isLoading;
	} Tex;

	typedef map<string, Tex> TexDictionary;

	static Graphics& getInstance() { static Graphics graph; return graph; };

	Graphics()
	{
		console()<<" graphics init..."<<endl;

		addToDictionary(SKIN_1, "background" , "mainSkinDesign\\bg.jpg");
		addToDictionary(SKIN_1, "background1" , "mainSkinDesign\\bg1.png");
		addToDictionary(SKIN_1, "background2" , "mainSkinDesign\\title.jpg");
	
		mainDesignLoadinsCounter = mainDesign.size();	
		loadingStack = 0;
	}

	void loadTextures(int id)
	{	
		loadingStack++;

		switch (id)
		{
			case SKIN_1:
				skinLoadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadSkinTextures, this ));
			break;

			case GAME_1:
				gameTexLoadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadGameTextures, this ));
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

private:

	void addToDictionary(int id, string key, string path)
	{	
		Tex value = {path, Texture(), false};
		mainDesign[key]  = value;
	}

	void loadSkinTextures()
	{
		for ( auto it = mainDesign.begin(); it != mainDesign.end(); it++)
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
					(*it).second.isLoading = true;
					if (--mainDesignLoadinsCounter == 0)
					{
						--loadingStack;
						completeHandler();
						skinLoadingSignal.disconnect();
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

	TexDictionary	mainDesign;

	int mainDesignLoadinsCounter;
	int loadingStack;

	ci::signals::connection skinLoadingSignal, gameTexLoadingSignal;


};

inline Graphics&	graphics() { return Graphics::getInstance(); };