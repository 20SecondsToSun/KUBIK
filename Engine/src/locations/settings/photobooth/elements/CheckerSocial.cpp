#include "CheckerSocial.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

CheckerSocial::CheckerSocial(const Rectf& rect, const IconPair& icons, const PhotoboothSettings::PhtTextID& id)
	:Checker(rect, icons, Color::hex(0xffff00),
	Color::hex(0x692a81))				
{
	setRadius(33);
	setStartX(-6);
	setFinishX(48);

	setBorderColorUnActive(ci::Color::hex(0x763493));
	setBorderColorActive(ci::Color::hex(0x7c3a99));

	event = CheckerSocialEventRef(new CheckerSocialEvent(isActive, id));	
}	

void CheckerSocial::mouseUp(ci::app::MouseEvent &mEvent)
{
	if(inButtonField(mEvent.getPos()))
	{
		swapActive();	

		CheckerSocialEventRef eventref = std::static_pointer_cast<CheckerSocialEvent>(event);
		eventref->setActive(isActive);		
		event = eventref;
		Sprite::mouseUp(mEvent);
	}
}