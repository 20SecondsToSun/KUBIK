#pragma once
#include "stdafx.h"

#include "gui/SimpleButton.h"

#include "Button.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		class MainTitleButton: public SimpleButton
		{
		public:	
			MainTitleButton(Rectf rect, int index):SimpleButton(rect)
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