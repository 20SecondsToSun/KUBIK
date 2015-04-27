#pragma once

#include "IMovie.h"

namespace kubik
{
	typedef std::shared_ptr<class VideoPlayer> VideoPlayerRef;

	class VideoPlayer : public IMovie
	{
	public:
		VideoPlayer(const  qtime::MovieGl& video);
		void draw() override;

	private:
		qtime::MovieGl video;
	};
}