#pragma once
#include "EventGUI.h"

namespace kubik
{	
	typedef shared_ptr<class Sprite> SpriteRef;	

	class Sprite 
	{

	public:
		Sprite();
		~Sprite(){};

		virtual void setParent(Sprite* parent);
		virtual void draw();
		virtual void drawLayout();
		virtual void setChildPosition(ci::Vec2i position);
		virtual void setPosition(ci::Vec2i position);		
		virtual void updateGlobalPosition();		
		virtual void setAlpha(float alpha);
		virtual ci::Vec2f getFullPosition();
		virtual ci::Vec2f getPosition();

		virtual void unActivateListeners();
		virtual void activateListeners();

		void addChild(SpriteRef child);
		void addChildFront(SpriteRef child);
		void removeChild(SpriteRef child);
		void removeAllChildren();
		
		ci::Vec2f getGlobalPosition();
		ci::Vec2f getParentPosition();
		
		template<typename T, typename Y>
		inline void	 connectEventHandler( T eventHandler, Y* obj, int event)
		{
			connectEventHandler0( std::bind( eventHandler, obj),  event );			
		}
		template<typename T, typename Y>
		void connectEventHandler( T eventHandler, Y* obj,  InteractEvent event = InteractEvent::TOUCH_UP)
		{
			if(mEventHandler != nullptr) return;
			connectEventHandler1( std::bind( eventHandler, obj, std::placeholders::_1 ), event );
			mouseUpSignalListener = mouseUpSignal.connect(std::bind( eventHandler, obj, std::placeholders::_1 ));
		}
	
		virtual void connectEventHandler0( const std::function<void()>& eventHandler, int event);
		void disconnectEventHandler(int event);
	
		virtual void connectEventHandler1( const std::function<void (EventGUIRef& )>& eventHandler, InteractEvent event);
		void disconnectEventHandler();

		virtual void mouseUp(ci::app::MouseEvent &_event);
		virtual void eventListener(EventGUIRef event);
	
		Sprite* getParent();
		Vec2f getLocalPosition();

		boost::signals2::signal<void(EventGUIRef &)> mouseUpSignal;		
		connection mouseUpSignalListener;

		void lockListeners();
		void unlockListeners();
		void callback(int id);

	protected:
		bool lock;
		float alpha;
		std::string name;
		ci::signals::connection mouseUpListener;		
		ci::Vec2f _localPosition, _parentPosition, _globalPosition;
		EventGUIRef event;			
		Sprite* parent;
		list<shared_ptr<Sprite>> displayList;

		std::function<void (EventGUIRef& )>	mEventHandler;	
		std::map<int, std::function<void()>> eventHandlerDic;	
	};
}