#pragma once

#include "IResourceScreenSaver.h"

namespace kubik
{
	class VideoScreenSaver:public IResourceScreenSaver
	{
	public:
		VideoScreenSaver(const ci::qtime::MovieGl& movie);
		void draw();
		void start();
		void stop();

	private:
		ci::qtime::MovieGl movie;
	};
}