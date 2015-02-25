#pragma once

#include "TextTools.h"
#include "gui/SimpleSpriteButton.h"
#include "DesignData.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ImageQuadroButton> ImageQuadroButtonRef;

		class ImageQuadroButton: public SimpleSpriteButton
		{
		public:
			ImageQuadroButton(OneDesignItem item, const ci::Vec2f& pos);

			virtual void drawLayout();
			void setAlpha(float alpha);
			void setSelection(bool value);
			void swapSelection();
			OneDesignItem getItem() const;

		private:
			OneDesignItem item;
			gl::Texture textTex;
			bool selection;
		};	
	}
}