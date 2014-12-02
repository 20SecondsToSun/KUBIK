#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"
#include <boost/algorithm/string.hpp>
#include "TextureManager.h"
#include "IDrawable.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Graphics
{

public:

	static Graphics& getInstance() { static Graphics graph; return graph; };

	Graphics()
	{
		loadingCounter = 0 ;
	}

	void setLoadingTextures(OneBlockTexDictionary _textures)
	{	
		for ( auto it = _textures.begin(); it != _textures.end(); it++)
		{
			textures.push_back((*it).second);
		}
		
		loadingCounter = textures.size();
	}

	void load()
	{
		loadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::loadTextures, this ));
	}
	
	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(void)>& handler)
	{
		errorHandler = handler;
	}

	void removeCompleteListener()
	{
		//completeHandler = NULL;
	}

	void removeErrorListener()
	{
		//errorHandler = NULL;
	}

	void uloadLastGame()
	{

	}

private:

	void loadTextures()
	{
		if (textures.size() == 0)
		{
			completeHandler();						
			loadingSignal.disconnect();
			return;
		}

		for ( auto it = textures.begin(); it != textures.end(); it++)
		{
			if ((*it)->isLoading == false)
			{
				string url = getFullTexturePath((*it)->path);
			
				if(!ph::isTextureLoaded(url))
				{
					(*it)->tex = ph::fetchTexture(url);
				}
				else if (loadingCounter > 0)
				{
					(*it)->tex = ph::fetchTexture(url);
					(*it)->isLoading = true;
					
					if (--loadingCounter == 0)
					{
						completeHandler();						
						loadingSignal.disconnect();
						textures.clear();
						ph::clearTexture();
						break;
					}
				}
			}
		}
	}

	string getFullTexturePath(string url)
	{
		return getAssetPath(url).string();
	}	

	std::function<void()> completeHandler;
	std::function<void()> errorHandler;

	int loadingCounter;

	ci::signals::connection loadingSignal;

	vector<TexObject*> textures;
};

inline Graphics&	graphics() { return Graphics::getInstance(); };