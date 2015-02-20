#pragma once
#include "Checker.h"
#include "CheckerSocialEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class CheckerSocial> CheckerSocialRef;

		class CheckerSocial: public Checker
		{
		public:
			CheckerSocial(Rectf rect, IconPair icons, PhotoboothSettings::PhtTextID id):
				Checker(rect, icons, Color::hex(0xffff00),
				Color::hex(0x692a81))				
			{
				setRadius(33);
				setStartX(-6);
				setFinishX(48);

				setBorderColorUnActive(ci::Color::hex(0x763493));
				setBorderColorActive(ci::Color::hex(0x7c3a99));

				event = CheckerSocialEventRef(new CheckerSocialEvent(isActive, id));	
			}	

			virtual void mouseUp(ci::app::MouseEvent &_event)
			{
				if(inButtonField(_event.getPos()))
				{
					swapActive();	

					CheckerSocialEventRef eventref = static_pointer_cast<CheckerSocialEvent>(event);
					eventref->setActive(isActive);		
					event = eventref;
					Sprite::mouseUp(_event);
				}
			}	
		};
	}
}