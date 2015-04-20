#pragma once

#include "Resources.h"
#include "IScreen.h"
#include "FileTools.h"

namespace kubik
{
	typedef std::shared_ptr<class ImageSequencer> ImageSequencerRef;

	class ImageSequencer
	{
	public:
		ImageSequencer();		
		void draw();
		void setImages(const std::vector<ci::gl::Texture>& images);	
		void setPosition(const ci::Vec2f& vec);

	private:
		std::vector<ci::gl::Texture> images;
		size_t index;
		ci::Vec2f position;
	};
}