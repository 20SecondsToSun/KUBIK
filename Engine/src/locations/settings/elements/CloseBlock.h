#pragma once
#include "stdafx.h"
#include "IDrawable.h"
#include "IDispatcher.h"
#include "Button.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock: public IDispatcher
		{
		public:	
			virtual void draw()
			{
				closeBtn->draw();			
			}		

			void createBtn(Texture tex)
			{
				closeBtn = ButtonRef(new Button(tex, position));	
				closeBtn->addMouseUpListener(&CloseBlock::mouseUpFunction, this);
			}		
		
		private:				
			ButtonRef closeBtn;
		};

		typedef std::shared_ptr<CloseBlock> CloseBlockRef;
	}
}