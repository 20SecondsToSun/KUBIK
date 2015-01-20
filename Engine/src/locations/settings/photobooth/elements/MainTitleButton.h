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
			MainTitleButton(int index):Button(Rectf(0,0,0,0))
			{
				event = OpenPhotoBoothLayoutEventRef(new OpenPhotoBoothLayoutEvent(index));		
			}	
		};

		typedef std::shared_ptr<MainTitleButton> MainTitleButtonRef;
	}
}