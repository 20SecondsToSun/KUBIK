#pragma once

using namespace ci::signals;
using namespace std;

namespace kubik
{
	class IDispatcher
	{

	public:
		void addMouseUpListener()
		{
			mouseUpListener = getWindow()->connectMouseUp(&IDispatcher::mouseUp, this);				
		}

		void removeMouseUpListener()
		{
			mouseUpListener.disconnect();			
		}	

		virtual void mouseUp(MouseEvent &event)
		{	
			for (auto comp : displayList)			
				comp->mouseUpHandler(event.getPos());
		}

	protected:
		list<IButtonRef> displayList;
	private:
		connection mouseUpListener;
	};
}