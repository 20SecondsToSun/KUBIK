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
			typedef std::shared_ptr<class FilterButton> FilterButtonRef;

			class FilterButton: public SimpleSpriteButton
			{
				int sizeID;
				int filterId;
				float scale;
				ci::gl::Texture photoTex;
				shaders::imagefilters::BaseShaderRef shader;

			public:	
				FilterButton(int filterId, const ci::Rectf& rectf, int sizeID = -1);

				void drawLayout();
				void setSizeID(int id);				
				void setTexture(const ci::gl::Texture& tex);
				void setScale(float scale);				
				int getSizeID() const;
			};			
		}
	}
}