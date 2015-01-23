#pragma once

#include "gui/SimpleButton.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class DecorateButton: public SimpleButton
	{	
		SimpleButtonRef simpleBtn; 

	public:
		DecorateButton(SimpleButtonRef simpleBtn):simpleBtn(simpleBtn)
		{

		}

		void draw() 
		{
			simpleBtn->draw();
		}

	};

	typedef shared_ptr<DecorateButton> DecorateButtonRef;
}