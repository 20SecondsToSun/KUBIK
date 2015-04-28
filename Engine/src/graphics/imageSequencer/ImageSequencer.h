#pragma once

#include "Resources.h"
#include "IScreen.h"
#include "FileTools.h"
#include "IMovie.h"

namespace kubik
{
	typedef std::shared_ptr<class ImageSequencer> ImageSequencerRef;

	class ImageSequencer : public IMovie
	{
	public:
		ImageSequencer(const std::vector<ci::gl::Texture>& images);
		void draw() override;
		float getHeight() override;
		float getWidth()override;

	private:
		std::vector<ci::gl::Texture> images;
		size_t index;
	};
}