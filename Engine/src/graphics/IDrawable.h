#pragma once

namespace kubik
{
	class IDrawable
	{
	public:			
		virtual void draw() = 0;	
		virtual void setPosition(ci::Vec2i position)		
		{
			this->position = position;
		}
	protected:
		ci::Vec2i position;
	};

	typedef std::shared_ptr<IDrawable> IDrawableRef;
}