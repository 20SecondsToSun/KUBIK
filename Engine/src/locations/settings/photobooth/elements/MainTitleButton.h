#pragma once
#include "stdafx.h"
#include "IDrawable.h"
#include "IDispatcher.h"
#include "Button.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		class MainTitleButton: public Button
		{
		public:	
			MainTitleButton(Rectf rect, int index):Button(rect)
			{
				event = OpenPhotoBoothLayoutEventRef(new OpenPhotoBoothLayoutEvent(index));		
			}	
		};

		typedef std::shared_ptr<MainTitleButton> MainTitleButtonRef;
	}
}