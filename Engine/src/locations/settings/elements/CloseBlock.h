#pragma once
#include "stdafx.h"
#include "IDrawable.h"
#include "IDispatcher.h"
#include "Button.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock: public Button
		{
		public:	
			CloseBlock(Vec2i position, Texture tex):Button(tex, position)
			{
				event = CloseConfigEventRef(new CloseConfigEvent());		
			}	
		};

		typedef std::shared_ptr<CloseBlock> CloseBlockRef;
	}
}