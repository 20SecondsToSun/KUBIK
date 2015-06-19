#pragma once
#include "IResourceScreenSaver.h"

namespace kubik
{
	class ImageScreenSaver : public IResourceScreenSaver
	{
	public:
		ImageScreenSaver(const ci::gl::Texture& texture);
		void draw();
		void start();
		void stop();

	private:
		ci::gl::Texture texture;
	};
}