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
	};

	inline void setScreenShot(const ci::gl::Texture& texture) 
	{ 
		return ScreenStorage::getInstance().setScreenShot(texture);
	};

	inline ci::gl::Texture getScreenShot() 
	{ 
		return ScreenStorage::getInstance().getScreenShot();
	};
}