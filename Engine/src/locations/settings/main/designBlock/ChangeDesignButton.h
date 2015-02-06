#pragma once

#include "ImageQuadroButton.h"
#include "DesignData.h"
#include "ChangeDesignEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ChangeDesignButton> ChangeDesignButtonRef;

		class ChangeDesignButton: public ImageQuadroButton
		{
		public:
			ChangeDesignButton(OneDesignItem item, ci::Vec2f pos)
				:ImageQuadroButton(item, pos)
			{
				event = ChangeDesignEventRef(new ChangeDesignEvent(item));
			}		

		};	
	}
}