#pragma once

#include "gui/SimpleSpriteButton.h"
#include "PhotoFilterEvent.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class FilterButton: public SimpleSpriteButton
			{
				int sizeID;
				int filterId;
				ci::gl::Texture photoTex;
				float scale;

				float maskSizeInit, maskSizeFinal;
				ci::Anim<float> maskSize;

				shaders::imagefilters::MaskShaderRef maskShader;

			public:	
				FilterButton(int filterId, ci::Rectf rectf, int sizeID = -1);
				void drawLayout();
				void setSizeID(int id);
				int getSizeID() const;
				void setTexture(const ci::gl::Texture& tex);
				void setScale(float scale);
				void showAnimate(float time, float delay);
			};

			typedef shared_ptr<FilterButton> FilterButtonRef;
		}
	}
}