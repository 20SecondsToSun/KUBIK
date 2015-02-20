#pragma once
#include "Checker.h"
#include "HashCheckerEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class HashChecker> HashCheckerRef;

		class HashChecker: public Checker
		{
		public:
			HashChecker(ci::Rectf rect, IconPair icons):
				Checker(rect, icons, ci::Color::hex(0xffff00), ci::Color::hex(0x522d02))				
			{
				setRadius(33);
				setStartX(-7);
				setFinishX(48);
				setBorderColorUnActive(ci::Color::hex(0xa05b09));
				setBorderColorActive(ci::Color::hex(0xbc6d10));
				
				event = HashCheckerEventRef(new HashCheckerEvent(isActive));	
			}	

			virtual void mouseUp(ci::app::MouseEvent &_event)
			{
				if(inButtonField(_event.getPos()))
				{
					swapActive();	

					HashCheckerEventRef eventref = static_pointer_cast<HashCheckerEvent>(event);
					eventref->setActive(isActive);		
					event = eventref;
					Sprite::mouseUp(_event);
				}
			}	
		};
	}
}