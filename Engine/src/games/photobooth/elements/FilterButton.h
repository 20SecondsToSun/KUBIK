#pragma once

#include "gui/SimpleSpriteButton.h"
#include "PhotoFilterEvent.h"

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
		public:	
			FilterButton(int filterId, ci::Rectf rectf)
				:SimpleSpriteButton(rectf, PhotoFilterEventRef(new PhotoFilterEvent(filterId)))
			{		
				
			}		

			void drawLayout()
			{				
				gl::color(Color::white());	
				gl::color(color);	
				gl::drawSolidRect(buttonArea);
			}
		};

		typedef shared_ptr<FilterButton> FilterButtonRef;
	}
}