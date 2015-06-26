#pragma once
#include "IMovie.h"
#include "ImageSequencer\ImageSequencer.h"
#include "videoplayer\VideoPlayer.h"

namespace kubik
{
	class MovieLoader
	{
	public:
		static MovieLoader& getInstance();

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

		IMovieRef getMovie(MovieLoaderStruct loaderStruct, ImageResourceDictionary imageDic, VideoResourceDictionary videoDic);
		std::vector<ci::gl::Texture> getPreloaderImages(int size, const std::string& name, ImageResourceDictionary imageDic);
		MovieLoaderStruct getMovieStruct(const std::string& templatePath, const std::string& name);

	};
	inline MovieLoader& movieLoader() { return MovieLoader::getInstance(); };
}