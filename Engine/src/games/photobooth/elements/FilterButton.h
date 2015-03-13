#pragma once

#include "gui/SimpleSpriteButton.h"
#include "PhotoFilterEvent.h"
#include "shaders/ShaderTool.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	namespace games
	{
		class FilterButton: public SimpleSpriteButton
		{
			int sizeID;
			int filterId;
			ci::gl::Texture photoTex;
			float scale;

		public:	
			FilterButton(int filterId, ci::Rectf rectf, int sizeID = -1)
				:SimpleSpriteButton(rectf, PhotoFilterEventRef(new PhotoFilterEvent(filterId))), sizeID(sizeID),
				filterId(filterId)
			{		
				
			}		

			void drawLayout()
			{	
				using namespace shaders::imagefilters;
				gl::color(Color::white());	
				gl::color(color);	
				//gl::drawSolidRect(buttonArea);
				auto shader = shadertool().get((ShaderTool::FilterType)filterId);
				
				if(photoTex)
					shader->render(photoTex);
			}

			void setSizeID(int id)
			{
				this->sizeID = id;
			}

			int getSizeID()
			{
				return this->sizeID;
			}

			void setTexture(ci::gl::Texture tex)
			{
				photoTex = tex;
			}

			void setScale(float scale)
			{
				this->scale = scale;
			}
		};

		typedef shared_ptr<FilterButton> FilterButtonRef;
	}
}