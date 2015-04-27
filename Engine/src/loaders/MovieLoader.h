#pragma once

#include "IMovie.h"
#include "ImageSequencer\ImageSequencer.h"
#include "videoplayer\VideoPlayer.h"

namespace kubik
{
	class MovieLoader
	{
	public:
		static MovieLoader& getInstance()
		{
			static MovieLoader mLoader;
			return mLoader;
		};

		enum MovieLoaderType
		{
			IMAGE_SEQUENCE,
			VIDEO
		};

		class MovieLoaderStruct
		{
		public:
			std::string name;
			MovieLoaderType type;	
			std::vector<std::string> paths;
		};

		IMovieRef getMovie(MovieLoaderStruct loaderStruct, ImageResourceDictionary imageDic, VideoResourceDictionary videoDic)
		{
			IMovieRef preloader;
		
			if (loaderStruct.type == IMAGE_SEQUENCE)
			{
				auto preloaderSeq = getPreloaderImages(loaderStruct.paths.size(), loaderStruct.name, imageDic);
				preloader = ImageSequencerRef(new ImageSequencer(preloaderSeq));
				preloader->setPosition(Vec2f(0.5f * (getWindowWidth() - preloaderSeq[0].getWidth()), 0.0f));
			}
			else if (loaderStruct.type == VIDEO)
			{
				console() << "CREATE VIDEO PRELOADER!!!!!!!!!!!!!!!" << endl;
				auto video = videoDic[loaderStruct.name]->get();
				preloader = VideoPlayerRef(new VideoPlayer(video));
				preloader->setPosition(Vec2f(0.5f * (getWindowWidth() - video.getWidth()), 0.0f));
			}
			else
			{
				//todo create null object
			}
			// clear paths
			return  preloader;
		};

		std::vector<ci::gl::Texture> getPreloaderImages(int size, string name, ImageResourceDictionary imageDic)
		{
			std::vector<ci::gl::Texture> preloaderSeq;
			for (int i = 0; i < size; i++)
				preloaderSeq.push_back(imageDic[name + to_string(i)]->get());

			return preloaderSeq;
		};

		MovieLoaderStruct getMovieStruct(const std::string& templatePath, const string& name)
		{
			MovieLoaderStruct mlStruct;
			mlStruct.name = name;

			auto videoPath = fileTools().getVideoPath(templatePath);
			if (!videoPath.empty())
			{
				mlStruct.type = MovieLoader::VIDEO;
				mlStruct.paths.push_back(videoPath);
			}
			else
			{
				auto files = fileTools().getAllImagePaths(templatePath);

				if (!files.empty())
				{
					mlStruct.type = MovieLoader::IMAGE_SEQUENCE;
					mlStruct.paths = files;
				}
			}

			return mlStruct;
		}

	};
	// helper function(s) for easier access 
	inline MovieLoader& movieLoader() { return MovieLoader::getInstance(); };
}