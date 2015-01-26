#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "InstagramClientInterface.h"
#include "InstagramClient.h"


using namespace std;
using namespace mndl::curl;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

namespace kubik
{
	namespace instagram
	{		
		

		class InstagramThreadClient: public InstagramClient
		{
		public:	
			InstagramThreadClient(string clientID):InstagramClient(clientID)
			{
			}

			virtual void loadPopularMedias()
			{	
				loadingStatus = LOADING;
				loadingSignal = App::get()->getSignalUpdate().connect(bind(&InstagramThreadClient::waitLoadingComplete, this));	
				instaThread = ThreadRef( new boost::thread( bind( &InstagramThreadClient::_loadPopularMedias, this , clientID) ) );
			}	

			signal<void(void)> LoadingCompleteSignal;

		private:
			void waitLoadingComplete()
			{
				if(loadingStatus == LOADED)
				{					
					//instaThread->join();
					loadingSignal.disconnect();		
					//LoadingCompleteSignal();
				}
				else if(loadingStatus == LOADING_ERROR)
				{
					//instaThread->join();
					loadingSignal.disconnect();	
					console()<<"=======LOADING_ERROR========="<<endl;
					//LoadingErrorSignal(ExcFileDoesNotExist());
				}
			}

			void _loadPopularMedias(string clientID)
			{				
				try
				{
					console()<<"=======loading========="<<endl;
					console()<<"clientID-------:  "<<clientID<<endl;
					//InstagramClient::loadPopularMedias(clientID);
					loadingStatus = LOADED;
				}
				catch(...)
				{
					loadingStatus = LOADING_ERROR;
				}				
			}		

			connection loadingSignal;
			ThreadRef instaThread;
			//InstagramClientRef instagramClient;

			enum
			{
				LOADING,
				LOADED,
				LOADING_ERROR
			}
			loadingStatus;	
		};

		typedef shared_ptr<InstagramThreadClient> InstagramThreadClientRef;
	}
}