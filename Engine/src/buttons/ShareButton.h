#pragma once

#include "ButtonText.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class ShareButton: public ButtonText
	{
	public:
		typedef boost::signals2::signal<void(ShareButton&)> ButtonSignal;	
		ButtonSignal mouseUpSignal;

		ShareButton(int filterId, Rectf rectf, string text, Font font):ButtonText(rectf, text, font)
		{		
			this->filterId = filterId;
		}

		int getFilterId()
		{
			return filterId;
		}

		void mouseUpHandler( Vec2i vec)
		{
			if(buttonArea.contains(vec))
				mouseUpSignal(*this);
		}

	private:

		int filterId;	
	};

	typedef shared_ptr<ShareButton> ShareButtonRef;
}