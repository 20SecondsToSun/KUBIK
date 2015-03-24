#pragma once

#include "Resources.h"
#include "IScreen.h"

namespace kubik
{
	typedef std::shared_ptr<class Preloader> PreloaderRef;

	class Preloader:public IScreen
	{
	public:
		Preloader(const ci::Vec2f& position);
		Preloader();

		virtual void draw() override;

		void setPosition(const ci::Vec2f& position);
		void init(ISettingsRef config) override;
		void reset() override;
		void setBackground(const gl::Texture& texture);

	protected:
		void setTextures();

	private:
		ci::Vec2f position;
		ci::gl::Texture tex, background;
	};	
}