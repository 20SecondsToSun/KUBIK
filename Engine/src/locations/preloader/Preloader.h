#pragma once

#include "Resources.h"
#include "IScreen.h"
#include "FileTools.h"
#include "ImageSequencer\ImageSequencer.h"
#include "videoplayer\VideoPlayer.h"

namespace kubik
{
	typedef std::shared_ptr<class Preloader> PreloaderRef;

	class Preloader:public IScreen
	{
	public:
		Preloader(const ci::Vec2f& position);
		Preloader();
		void create(const ci::Vec2f& position);

		virtual void draw() override;

		void setPosition(const ci::Vec2f& position);
		void init(ISettingsRef config) override;
		void reset() override;
		void setBackground(const gl::Texture& texture);

	protected:
		void setTextures();

	private:		
		ci::Vec2f position;
		ci::gl::Texture background;
		IMovieRef preloader;		
	};	
}