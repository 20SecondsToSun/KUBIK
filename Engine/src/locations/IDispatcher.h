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

		bool isMouseUpSet;
		boost::signals2::signal<void(EventRef &)> mouseUpSignal;	
		//boost::signals2::signal<void(StatisticEventRef &)> mouseUpSignal;	

		virtual void mouseUpFunction( shared_ptr<kubik::Event> event )
		{
			//console()<<"Event:::ButtonHolder::"<<event->getMsg()<<endl;
			mouseUpSignal(event);
		}

	protected:
		list<shared_ptr<IDispatcher>> displayList;

	private:
		connection mouseUpListener, mouseUpSignalListener;
	};

	//class Button: public IDispatcher
	//{
	//public:
	//	Button(int id, Rectf field, Color color):id(id), field(field), color(color)
	//	{
	//		event = shared_ptr<Event>(new Event("buttonClick " +to_string(id)));
	//	}

	//	virtual void mouseUpHandler(ci::Vec2i pos)
	//	{
	//		if(field.contains(pos))
	//		{
	//			mouseUpSignal(event);
	//			//console()<<"handler BUTTON()"<<id<<endl;
	//		}
	//	}

	//	virtual void draw()
	//	{
	//		gl::color(color);
	//		gl::drawSolidRect(field);
	//		gl::color(Color::white());
	//		IDispatcher::draw();
	//	}

	//private:
	//	int id;
	//	Rectf field;
	//	Color color;
	//	shared_ptr<Event> event;
	//};

	//class ButtonHolder:  public IDispatcher
	//{
	//public:
	//	ButtonHolder()
	//	{
	//		event = shared_ptr<Event>(new Event("HolderClick"));
	//		disp = shared_ptr<Button>(new Button(1, Rectf(0,0,500,500), Color(1,0,0)));
	//		disp1= shared_ptr<Button>(new Button(2, Rectf(0,0,300,300), Color(0,1,0)));
	//		addChild(disp);
	//		addChild(disp1);	
	//		disp->addMouseUpListener(&ButtonHolder::mouseUpFunction, this);
	//		disp1->addMouseUpListener(&ButtonHolder::mouseUpFunction, this);
	//		i = 0;
	//		//disp->mouseUpSignal.connect( std::bind( &SpaceColonizationApp::mHandler, this, std::placeholders::_1 ));	
	//		//disp1->mouseUpSignal.connect( std::bind( &SpaceColonizationApp::mHandler, this, std::placeholders::_1 ));	
	//	}

	//	virtual void mouseUpHandler(ci::Vec2i pos)
	//	{
	//		if(i<=1)
	//		mouseUpSignal(event);
	//		if(i == 0)
	//			disp->removeMouseUpListener();
	//		if(i==1)
	//			disp1->removeMouseUpListener();
	//		i++;
	//		//console()<<"test"<<endl;
	//	}

	//	virtual void draw()
	//	{
	//		gl::color(Color::black());
	//		gl::drawSolidRect(Rectf(0,0,1200,1200));
	//		gl::color(Color::white());
	//		IDispatcher::draw();
	//	}
	//private:
	//	shared_ptr<Event> event;
	//	shared_ptr<IDispatcher> disp, disp1;
	//	int i;
	//};

	//class MainHolder:  public IDispatcher
	//{
	//public:
	//	MainHolder()
	//	{
	//		event = shared_ptr<Event>(new Event("MainHolderClick"));
	//		btnh = shared_ptr<ButtonHolder>(new ButtonHolder());
	//		btnh->addMouseUpListener(&ButtonHolder::mouseUpFunction, this);
	//		addChild(btnh);
	//	}
	//	virtual void mouseUpHandler(ci::Vec2i pos)
	//	{
	//		mouseUpSignal(event);
	//	}
	//	private:
	//	shared_ptr<Event> event;
	//	shared_ptr<ButtonHolder> btnh;
	//};
}