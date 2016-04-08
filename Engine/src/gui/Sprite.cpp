#include "gui/Sprite.h"

using namespace kubik;
using namespace ci;
using namespace ci::app;

Sprite::Sprite():_globalPosition(ci::Vec2f::zero()),
	_localPosition(ci::Vec2f::zero()),
	_parentPosition(ci::Vec2f::zero()),
	alpha(1),
	name("default"),
	parent(NULL),
	mEventHandler(nullptr),
	lock(false)
{

}

void Sprite::addChild(SpriteRef child)
{	
	child->setParent(this);
	child->setChildPosition(_localPosition);
	child->setAlpha(alpha);			
	displayList.push_back(child);
}

void Sprite::addChildFront(SpriteRef child)
{
	child->setParent(this);
	child->setChildPosition(_localPosition);
	child->setAlpha(alpha);			
	displayList.push_front(child);
}

void Sprite::removeChild(SpriteRef child)
{
	displayList.remove(child);
}

void Sprite::removeAllChildren()
{
	displayList.clear();
}

void Sprite::setParent(Sprite* parent)
{
	this->parent = parent;
}

void Sprite::draw()
{			
	gl::pushMatrices();				
	gl::translate(getGlobalPosition());
	drawLayout();
	gl::popMatrices();

	for (auto comp : displayList)
		comp->draw();
}

void Sprite::drawLayout()
{

}

void Sprite::setChildPosition(ci::Vec2i position)
{
	_parentPosition = getFullPosition();

	updateGlobalPosition();
	for (auto comp : displayList)			
		comp->setChildPosition(position);
}

void Sprite::setPosition(ci::Vec2i position)		
{
	_localPosition = position;		
	_parentPosition = getFullPosition();

	updateGlobalPosition();
	for (auto comp : displayList)			
		comp->setChildPosition(position);		
}

ci::Vec2f Sprite::getFullPosition()		
{
	ci::Vec2f _position (ci::Vec2f::zero());

	if(parent)
		_position = parent->getGlobalPosition();

	return _position;
}

void Sprite::updateGlobalPosition()
{
	_globalPosition = _localPosition + _parentPosition;
}

ci::Vec2f Sprite::getGlobalPosition()
{
	return _globalPosition;
}

ci::Vec2f Sprite::getParentPosition()
{
	return _parentPosition;
}

ci::Vec2f Sprite::getPosition()		
{
	return _localPosition;			
}

void Sprite::setAlpha(float alpha)
{
	this->alpha = alpha;
	for (auto comp : displayList)			
		comp->setAlpha(alpha);
}

void Sprite::connectEventHandler0( const std::function<void()>& eventHandler, int event)
{
	eventHandlerDic[event] = eventHandler;
}

void Sprite::disconnectEventHandler(int event)
{
	eventHandlerDic[event] = nullptr;
}

void Sprite::connectEventHandler1( const std::function<void (EventGUIRef& )>& eventHandler, InteractEvent event)
{
	if(mEventHandler == nullptr)
	{
		mEventHandler = eventHandler;
		mouseUpListener = getWindow()->connectMouseUp(&Sprite::mouseUp, this);							
	}		
}

void Sprite::disconnectEventHandler()
{
	if(mEventHandler != nullptr)
	{
		mEventHandler = nullptr;
		mouseUpListener.disconnect();
		mouseUpSignalListener.disconnect();
	}
}	

void Sprite::mouseUp(ci::app::MouseEvent &_event)
{			
	if(lock) return;

	if (!mEventHandler)return;
	mEventHandler(event);

	Sprite* _parent = parent;

	while(_parent && !_parent->mEventHandler)
	{
		_parent = _parent->getParent();
	}

	if(_parent)//&& !_parent->mEventHandler)
	{
		_parent->mouseUpSignal(event);
		//			
	}
}

void Sprite::eventListener(EventGUIRef event)
{

}					

void Sprite::unActivateListeners()
{
	for (auto comp : displayList)
		comp->unActivateListeners();			
}

void Sprite::activateListeners()
{			
	for (auto comp : displayList)
		comp->activateListeners();			
}

Sprite* Sprite::getParent()
{			
	return parent;			
}

ci::Vec2f Sprite::getLocalPosition()
{
	return _localPosition;
}

void Sprite::lockListeners()
{
	for (auto comp : displayList)
		comp->lockListeners();

	lock = true;
}

void Sprite::unlockListeners()
{
	for (auto comp : displayList)
		comp->unlockListeners();

	lock = false;
}

void Sprite::callback(int id)
{
	if(eventHandlerDic[id])
		eventHandlerDic[id]();
}