#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"
#include <boost/algorithm/string.hpp>
#include "cinder/ImageIo.h"
#include <boost/thread.hpp>
#include "IDrawable.h"
#include "ServiceMessage.h"
#include "Types.h"

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

	void setLoadingTextures(Types::OneBlockTexDictionary _textures)
	{	
		for ( auto it = _textures.begin(); it != _textures.end(); it++)
		{
			loadingRes.push_back((*it).second);
		}

		loadingCounter = loadingRes.size();
	}

	void load()
	{
		for (auto res: loadingRes)
		{
			console()<<" path::  "<<res->path<<endl;
		}

		loadingStatus = SCREEN_SAVER_LOADING;
		loadingSignal = App::get()->getSignalUpdate().connect( bind( &Graphics::waitLoadingComplete, this ));
		loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&Graphics::loadTextures, this));

	}

	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(ServiceMessage)>& handler)
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

	void waitLoadingComplete()
	{
		if(loadingStatus == SCREEN_SAVER_LOADED)
		{
			loadingThread->join();
			loadingSignal.disconnect();
			loadingRes.clear();
			completeHandler();
		}
		else if(loadingStatus == SCREEN_SAVER_LOADING_ERROR)
		{
			loadingThread->join();
			loadingSignal.disconnect();
			loadingRes.clear();
			ServiceMessage msg(101);			
			errorHandler(msg);
		}
	}

	void loadTextures()
	{	
		for (auto res: loadingRes)
		{
			if(res->resourceType == resourceType::IMAGE)
			{
				try 
				{
					console()<<"try image loaded  "<< res->path <<endl;
					Surface image = Surface(loadImage( ci::loadFile( res->path ) ));
					res->tex = image;
					console()<<"screen saver image loaded"<<endl;
				}
				catch( ... ) 
				{
					loadingStatus = SCREEN_SAVER_LOADING_ERROR;
					console() << "Unable to load the image." << std::endl;	
					break;
				}
			}
			else if(res->resourceType == resourceType::VIDEO)
			{
				try 
				{
					qtime::MovieGl movie = qtime::MovieGl( res->path);					
					console()<<"screen saver video loaded"<<endl;
					res->movie = movie;
				}
				catch( ... ) 
				{
					console() << "Unable to load the movie." << std::endl;
					loadingStatus = SCREEN_SAVER_LOADING_ERROR;
				}
			}
			else if(res->resourceType == resourceType::FONT)
			{
				try 
				{					
					Font font =  Font(loadFile(fs::path(res->path)), res->fontSize);
					res->font = font;
					console() << "font loaded." << std::endl;
				}
				catch( ... ) 
				{

				}
			}
		}

		if(loadingStatus == SCREEN_SAVER_LOADING)
			loadingStatus = SCREEN_SAVER_LOADED;
		
	}

	string getFullTexturePath(string url)
	{	
		return getAssetPath(url).string();
	}	

	std::function<void()> completeHandler;
	std::function<void(ServiceMessage)> errorHandler;

	int loadingCounter;

	ci::signals::connection loadingSignal;

	vector<Types::TexObject*> loadingRes;

	boost::shared_ptr<boost::thread>	loadingThread;
	
	enum
	{
		SCREEN_SAVER_LOADING,
		SCREEN_SAVER_LOADED,
		SCREEN_SAVER_LOADING_ERROR
	}
	loadingStatus;
	
};

inline Graphics&	graphics() { return Graphics::getInstance(); };