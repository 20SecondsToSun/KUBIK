#include "loaders/asynchTextureLoader/TextureStore.h"

#pragma once

namespace instagram
{
	class ImageGraphic
	{
	public:
		ImageGraphic();

		void setLowResSurface(const ci::Surface& surf);		
		void fadeIn(float from, float to);		
		void setSize(int size);
		void draw(const ci::Vec2f& vec);
		void drawStandartResImage(const ci::Vec2f& vec);
		void setLowResURL(const std::string& url);		
		void setStandartResURL(const std::string& url);
		void loadLowRes();
		void setResImage(const ci::Surface& surf);
		void loadStandartRes();
		std::string getStandartResURL() const;
		float getAlpha() const;

		std::string getLowResURL() const;
		ci::gl::Texture getStandartResImage();
		ci::Surface getLowResSurface();

	protected:
		ci::gl::Texture tex, bigtex;
		bool loadedthumb;

		ci::EaseFn animFunc;
		ci::gl::Texture standartSurfaceTex;
		ci::Anim<float> alpha;

		float animTime;		
		int width, height;

		std::string lowResURL;
		std::string standartResURL;
	};
}