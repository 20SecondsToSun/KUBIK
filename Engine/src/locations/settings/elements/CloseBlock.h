#pragma once
#include "stdafx.h"
#include "IDrawable.h"
#include "IDispatcher.h"
#include "Button.h"

namespace kubik
{
	namespace config
	{
		class CloseBlock:public IDrawable, public IDispatcher
		{
		public:	
			virtual void draw()
			{
				closeBtn->draw();
				gl::pushMatrices();
				gl::translate(position);				
				gl::popMatrices();
			}		

			void setCloseTexture(Texture tex)
			{
				closeTex = tex;				
			}

			void createBtn()
			{
				closeBtn = ButtonRef(new Button(closeTex, position));	
				connect_once(closeBtn->mouseUpSignal, bind(&CloseBlock::closeSignalInvoke, this, std::placeholders::_1));
				displayList.push_back(closeBtn);
			}

			void closeSignalInvoke(IButton& button)
			{			
				closeSignal(button);
			}				

			ButtonSignal  closeSignal;

		private:			
			Texture closeTex;
			ButtonRef closeBtn;
		};

		typedef std::shared_ptr<CloseBlock> CloseBlockRef;
	}
}