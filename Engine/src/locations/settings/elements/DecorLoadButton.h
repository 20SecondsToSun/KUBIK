#pragma once
#include "LoadButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class DecorLoadButton> DecorLoadButtonRef;

		class DecorLoadButton: public LoadButton
		{
		public:
			DecorLoadButton(const std::string& path, const ci::Rectf& rect, const TextItem& textItem, const ci::gl::Texture& icon);	
			virtual void drawLayout();
			void draw(float x, float y);
			virtual void draw();
		};	
	}
}