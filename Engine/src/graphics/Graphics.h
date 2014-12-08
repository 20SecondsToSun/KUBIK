#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "Types.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class Graphics
	{

	public:

		ci::signals::signal<void(void)> completeLoadingSignal;
		ci::signals::signal<void(KubikException)> errorLoadingSignal;

		void setLoadingTextures(Types::OneBlockTexDictionary _textures)
		{	
			for ( auto it = _textures.begin(); it != _textures.end(); it++)		
				loadingRes.push_back((*it).second);
		}

		void load()
		{
			loadingStatus = LOADING;
			loadingSignal = App::get()->getSignalUpdate().connect(bind(&Graphics::waitLoadingComplete, this));		
			loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&Graphics::loadTextures, this));
		}

	private:

		ci::signals::connection loadingSignal;
		vector<Types::TexObject*> loadingRes;
		boost::shared_ptr<boost::thread> loadingThread;

		enum
		{
			LOADING,
			LOADED,
			LOADING_ERROR
		}
		loadingStatus;	

		void waitLoadingComplete()
		{
			if(loadingStatus == LOADED)
			{
				loadingThread->join();
				loadingSignal.disconnect();
				loadingRes.clear();
				completeLoadingSignal();
			}
			else if(loadingStatus == LOADING_ERROR)
			{
				loadingThread->join();
				loadingSignal.disconnect();
				loadingRes.clear();
				errorLoadingSignal(ExcFileDoesNotExist());
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
						console()<<"try image load  "<< res->path <<endl;
						Surface image = Surface(loadImage( ci::loadFile( res->path ) ));
						res->tex = image;
						console()<<"image loaded"<<endl;
					}
					catch( ... ) 
					{
						loadingStatus = LOADING_ERROR;
						console() << "Unable to load the image." << std::endl;	
						break;
					}
				}
				else if(res->resourceType == resourceType::VIDEO)
				{
					try 
					{
						console()<<"try video load  "<< res->path <<endl;
						qtime::MovieGl movie = qtime::MovieGl( res->path);					
						console()<<"video loaded"<<endl;
						res->movie = movie;
					}
					catch( ... ) 
					{
						console() << "Unable to load the movie." << std::endl;
						loadingStatus = LOADING_ERROR;
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
						loadingStatus = LOADING_ERROR;
					}
				}
			}

			if(loadingStatus == LOADING)
				loadingStatus = LOADED;			
		}		
	};
}