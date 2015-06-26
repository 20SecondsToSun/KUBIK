#pragma once

#include "gui/SimpleSpriteButton.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class MainTitleButton> MainTitleButtonRef;

		class MainTitleButton: public SimpleSpriteButton
		{
		public:	
			MainTitleButton(const ci::Rectf& rect, int index);
			virtual void draw();
		};		
	}
}