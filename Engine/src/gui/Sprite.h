#pragma once
#include "EventGUI.h"

namespace kubik
{	
	typedef shared_ptr<class Sprite> SpriteRef;	

	class Sprite 
	{

	public:
		Sprite()
			:_globalPosition(ci::Vec2f::zero()),
			 _localPosition(ci::Vec2f::zero()),
			 _parentPosition(ci::Vec2f::zero()),
			 alpha(1),
			 name("default"),
			 parent(NULL),
			 mEventHandler(nullptr),
			 lock(false)
		{
		}

		void addChild(SpriteRef child)
		{	
			child->setParent(this);
			child->setChildPosition(_localPosition);
			child->setAlpha(alpha);			
			displayList.push_back(child);
		}	

		void removeChild(SpriteRef child)
		{
			displayList.remove(child);
		}


		virtual void setParent(Sprite* parent)
		{
			this->parent = parent;
		}

		virtual void draw()
		{			
			gl::pushMatrices();				
			gl::translate(getGlobalPosition());
				drawLayout();
			gl::popMatrices();

			for (auto comp : displayList)
				comp->draw();
		}

		virtual void drawLayout()
		{

		}

		virtual void setChildPosition(ci::Vec2i position)
		{
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);
		}

		virtual void setPosition(ci::Vec2i position)		
		{
			_localPosition = position;		
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);		
		}

		virtual ci::Vec2f getFullPosition()		
		{
			ci::Vec2f _position (ci::Vec2f::zero());
			
			if(parent)
				_position = parent->getGlobalPosition();

			return _position;
		}

		virtual void updateGlobalPosition()
		{
			_globalPosition = _localPosition + _parentPosition;
		}

		ci::Vec2f getGlobalPosition()
		{
			return _globalPosition;
		}

		ci::Vec2f getParentPosition()
		{
			return _parentPosition;
		}

		virtual ci::Vec2f getPosition()		
		{
			return _localPosition;			
		}

		virtual void setAlpha(float alpha)
		{
			this->alpha = alpha;
			for (auto comp : displayList)			
				comp->setAlpha(alpha);
		}

		template<typename T, typename Y>
		inline void	 connectEventHandler( T eventHandler, Y* obj, int event)
		{
			connectEventHandler0( std::bind( eventHandler, obj),  event );			
		}
	
		virtual void connectEventHandler0( const std::function<void()>& eventHandler, int event)
		{
			eventHandlerDic[event] = eventHandler;
		}

		void disconnectEventHandler(int event)
		{
			eventHandlerDic[event] = nullptr;
		}	

		template<typename T, typename Y>
		inline void	 connectEventHandler( T eventHandler, Y* obj, InteractEvent event = InteractEvent::TOUCH_UP)
		{
			connectEventHandler1( std::bind( eventHandler, obj, std::placeholders::_1 ), event );
			mouseUpSignalListener = mouseUpSignal.connect(std::bind( eventHandler, obj, std::placeholders::_1 ));
		}

		virtual void connectEventHandler1( const std::function<void (EventGUIRef& )>& eventHandler, InteractEvent event)
		{
			if(mEventHandler == nullptr)
			{
				mEventHandler = eventHandler;
				mouseUpListener = getWindow()->connectMouseUp(&Sprite::mouseUp, this);							
			}		
		}

		void disconnectEventHandler( )
		{
			if(mEventHandler != nullptr)
			{
				mEventHandler = nullptr;
				mouseUpListener.disconnect();
				mouseUpSignalListener.disconnect();
			}
		}	

		virtual void mouseUp(ci::app::MouseEvent &_event)
		{			
			if(lock) return;

			mEventHandler(event);

			Sprite* _parent = parent;

			while(_parent && !_parent->mEventHandler)
			{
				_parent = _parent->getParent();
			}

			if(_parent )//&& !_parent->mEventHandler)
			{
				_parent->mouseUpSignal(event);
				//			
			}
		}

		virtual void eventListener(EventGUIRef event)
		{
			
		}					

		virtual void unActivateListeners()
		{
			for (auto comp : displayList)
				comp->unActivateListeners();			
		}

		virtual void activateListeners()
		{			
			for (auto comp : displayList)
				comp->activateListeners();			
		}

		Sprite* getParent()
		{			
			return parent;			
		}

		Vec2f getLocalPosition()
		{
			return _localPosition;
		}
			
		boost::signals2::signal<void(EventGUIRef &)> mouseUpSignal;		
		connection mouseUpSignalListener;

		void lockListeners()
		{
			for (auto comp : displayList)
				comp->lockListeners();

			lock = true;
		}

		void unlockListeners()
		{
			for (auto comp : displayList)
				comp->unlockListeners();

			lock = false;
		}

		void callback(int id)
		{
			if(eventHandlerDic[id])
				eventHandlerDic[id]();
		}

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