#pragma once

#include "Types.h"
#include "KubikException.h"
#include "ILoader.h"

namespace kubik
{
	typedef std::shared_ptr<class GraphicsLoader> GraphicsLoaderRef;

	class GraphicsLoader : public ILoader
	{
	public:
		void setLoadingTextures(IResourceDictionary textures);
		void setLoadingTextures(std::vector<IResourceBaseRef> textures);
		void load() override;

	private:
		boost::signals2::connection loadingSignal;
		std::vector<IResourceBaseRef> loadingRes;
		ThreadRef loadingThread;

		enum
		{
			LOADING,
			LOADED,
			LOADING_ERROR
		}
		loadingStatus;	

		void waitLoadingComplete();
		void loadTextures();
	};	
}