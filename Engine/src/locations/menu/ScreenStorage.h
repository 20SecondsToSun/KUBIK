#pragma once

namespace kubik
{
	class ScreenStorage
	{
		ci::gl::Texture texture;

	public:
		static ScreenStorage& getInstance() 
		{
			static ScreenStorage storage;
			return storage;
		};

		void setScreenShot(const ci::gl::Texture& texture)
		{
			this->texture = texture;
		}

		ci::gl::Texture getScreenShot()
		{
			return texture;
		}

		void drawScreenShot()
		{
			if (texture)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				gl::draw(texture);
				gl::enableAlphaBlending();
			}				
		}
	};

	inline void setScreenShot(const ci::gl::Texture& texture) 
	{ 
		return ScreenStorage::getInstance().setScreenShot(texture);
	};

	inline ci::gl::Texture getScreenShot() 
	{ 
		return ScreenStorage::getInstance().getScreenShot();
	};

	inline void drawScreenShot()
	{
		return ScreenStorage::getInstance().drawScreenShot();
	};
}