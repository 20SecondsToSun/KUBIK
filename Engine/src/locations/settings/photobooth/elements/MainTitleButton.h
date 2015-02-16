#pragma once

#include "stdafx.h"
#include "gui/SimpleSpriteButton.h"
#include "Button.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		class MainTitleButton: public SimpleSpriteButton
		{
		public:	
			MainTitleButton(Rectf rect, int index):SimpleSpriteButton(rect)
			{
				event = OpenPhotoBoothLayoutEventRef(new OpenPhotoBoothLayoutEvent(index));		
			}	

			virtual void draw()
			{

			}
		};

		typedef std::shared_ptr<MainTitleButton> MainTitleButtonRef;
	}
}