#include "HashChecker.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

HashChecker::HashChecker(const Rectf& rect, const IconPair& icons):Checker(rect, icons, Color::hex(0xffff00), Color::hex(0x522d02))				
{
	setRadius(33);
	setStartX(-7);
	setFinishX(48);
	setBorderColorUnActive(Color::hex(0xa05b09));
	setBorderColorActive(Color::hex(0xbc6d10));

	event = HashCheckerEventRef(new HashCheckerEvent(isActive));	
}	

void HashChecker::mouseUp(app::MouseEvent &mEvent)
{
	if(inButtonField(mEvent.getPos()))
	{
		swapActive();	

		HashCheckerEventRef eventref = static_pointer_cast<HashCheckerEvent>(event);
		eventref->setActive(isActive);		
		event = eventref;

		Sprite::mouseUp(mEvent);
	}
}