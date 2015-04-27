#pragma once

#include "Types.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace ci::signals;

namespace kubik
{
	class GraphicsLoader : public ILoader
	{

	public:
		void setLoadingTextures(IResourceDictionary _textures)
		{	
			for ( auto it = _textures.begin(); it != _textures.end(); it++)		
				loadingRes.push_back((*it).second);
		}

		void setLoadingTextures(vector<IResourceBaseRef> _textures)
		{
			for (auto it = _textures.begin(); it != _textures.end(); it++)
				loadingRes.push_back(*it);
		}

		void load() override
		{
			loadingStatus = LOADING;
			loadingSignal = App::get()->getSignalUpdate().connect(bind(&GraphicsLoader::waitLoadingComplete, this));		
			loadingThread = ThreadRef(new boost::thread(&GraphicsLoader::loadTextures, this));
		}

	private:
		connection loadingSignal;
		vector<IResourceBaseRef> loadingRes;
		ThreadRef loadingThread;

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
				LoadingCompleteSignal();
			}
			else if(loadingStatus == LOADING_ERROR)
			{
				loadingThread->join();
				loadingSignal.disconnect();
				loadingRes.clear();
				LoadingErrorSignal(ExcFileDoesNotExist());
			}
		}

		void loadTextures()
		{
			string loadPath;
			try 
			{
				for (auto res: loadingRes)
				{
					if(res->resourceType == resourceType::IMAGE)
					{					
						ImageResourceRef imageRes = static_pointer_cast<ImageResource>(res);	

						//console()<<"try image load  "<< res->path <<endl;
						loadPath = res->path;
						ci::Surface image = ci::Surface(loadImage(ci::loadFile(loadPath)));
						
						//image.setPremultiplied(true);
						imageRes->set(image);								
					}
					else if(res->resourceType == resourceType::VIDEO)
					{				
						VideoResourceRef videoRes = static_pointer_cast<VideoResource>(res);	

						//console()<<"try video load  "<< res->path <<endl;
						loadPath = res->path;
						qtime::MovieGl movie = qtime::MovieGl(loadPath);
						videoRes->set(movie);							
					}
					else if(res->resourceType == resourceType::FONT)
					{					
						FontResourceRef fontRes = static_pointer_cast<FontResource>(res);
						loadPath = res->path;
						//console()<<"try font load  "<< res->path <<endl;
						Font font = Font(loadFile(fs::path(loadPath)), fontRes->fontSize);
						fontRes->set(font);	
					}
				}
			}
			catch(...) 
			{
				loadingStatus = LOADING_ERROR;
				console() << "Unable to load the resource." << loadPath << endl;
			}

			if(loadingStatus == LOADING)
				loadingStatus = LOADED;			
		}		
	};

	typedef shared_ptr<GraphicsLoader> GraphicsLoaderRef;
}