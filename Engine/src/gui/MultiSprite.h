#pragma once
#include "cinder/ImageIO.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "Sprite.h"

namespace kubik
{	
	class MultiSprite: public Sprite
	{
	public:
		MultiSprite():Sprite()
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

		virtual void draw()
		{
			for (auto comp : displayList)			
				comp->draw();

			Sprite::draw();
		}	

		virtual void setPosition(ci::Vec2i position)		
		{
			_localPosition = position;		
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);			
		}	

		virtual void setChildPosition(ci::Vec2i position)
		{
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);
		}
	
		virtual void setAlpha(float alpha)
		{
			this->alpha = alpha;
			for (auto comp : displayList)			
				comp->setAlpha(alpha);
		}

		virtual void unActivateListeners()
		{
			for (auto comp : displayList)
				comp->unActivateListeners();

			Sprite::unActivateListeners();
		}

		virtual void activateListeners()
		{			
			for (auto comp : displayList)
				comp->activateListeners();

			Sprite::activateListeners();
		}

		template<typename T, typename Y>
		void connectEventHandler( T eventHandler, Y* obj )
		{
		//	connectEventHandler( std::bind( eventHandler, obj, std::placeholders::_1 ) );
			console()<<"main sprite connect"<<endl;
			mouseUpSignalListener = mouseUpSignal.connect(std::bind( eventHandler, obj, std::placeholders::_1 ));
		}	

	private:
		list<shared_ptr<Sprite>> displayList;
		connection mouseUpSignalListener;
	};

	typedef shared_ptr<MultiSprite> MultiSpriteRef;	
}