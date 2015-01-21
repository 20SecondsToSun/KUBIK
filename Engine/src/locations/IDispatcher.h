#pragma once
#include "Event.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	class IDispatcher;
	typedef shared_ptr<IDispatcher> IDispatcherRef;	

	class IDispatcher : public IDrawable
	{

	public:
		IDispatcher():isMouseUpSet(false)
		{
		}

		void addChild(shared_ptr<IDispatcher> child)
		{
			displayList.push_back(child);
		}

		template<typename T, typename Y>
		void addMouseUpListener( T fn, Y *inst )
		{
			if(isMouseUpSet) return;

			isMouseUpSet = true;
			mouseUpListener = getWindow()->connectMouseUp(&IDispatcher::mouseUp, this);							
			mouseUpSignalListener = mouseUpSignal.connect(std::bind( fn, inst, std::placeholders::_1 ));
		}
	
		void removeMouseUpListener()
		{
			mouseUpListener.disconnect();
			mouseUpSignalListener.disconnect();
			isMouseUpSet = false;
		}	

		virtual void mouseUp(MouseEvent &event)
		{	
			//for (auto comp : displayList)			
			//	comp->mouseUpHandler(event.getPos());
			mouseUpHandler(event.getPos());			
		}

		virtual void mouseUpHandler(ci::Vec2i pos)
		{	
			//console()<<"handler()"<<endl;
		}

		virtual void draw()
		{
			for (auto comp : displayList)			
				comp->draw();
		}

		virtual void mouseUpFunction(shared_ptr<kubik::Event> event )
		{
			mouseUpSignal(event);
		}

		virtual void activateListeners()
		{

		}

		virtual void setAlpha(float  alpha)
		{

		}

		virtual void unActivateListeners()
		{

		}

		bool isMouseUpSet;
		boost::signals2::signal<void(EventRef &)> mouseUpSignal;			

	protected:
		list<shared_ptr<IDispatcher>> displayList;

	private:
		connection mouseUpListener, mouseUpSignalListener;
	};
}