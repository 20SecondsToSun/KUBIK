#include "PhotoboothConfig.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

PhotoboothConfig::PhotoboothConfig(PhotoboothSettingsRef phbSettings)
	:GameSettingsSprite(), leftMargin(165), phbSettings(phbSettings)			
{					
	photoOverElements = PhotoOverElementsRef(new PhotoOverElements(phbSettings,	Color::hex(0x01a7fb), 0));
	photoCardStyles   = PhotoCardStyleRef(new PhotoCardStyle(phbSettings, Color::hex(0x1f93e9), 1));
	photoFilters	  = PhotoFiltersRef(new PhotoFilters(phbSettings, Color::hex(0x4976d2), 2));
	sharing			  = SharingRef(new Sharing(phbSettings, Color::hex(0x6d5dbd), 3));

	addChild(photoOverElements);
	addChild(photoCardStyles);	
	addChild(photoFilters);
	addChild(sharing);						
}	

void PhotoboothConfig::mouseUpHandler(EventGUIRef& mEvent)
{
	EventGUI *ev = mEvent.get();
	if(!ev)	return;

	if(typeid(*ev) == typeid(OpenPhotoBoothLayoutEvent))
	{					
		OpenPhotoBoothLayoutEventRef event = static_pointer_cast<OpenPhotoBoothLayoutEvent>(mEvent);	
		setOpenItem(event->getlayoutIndex());
	}
	else if(typeid(*ev) == typeid(CheckerSocialEvent))
	{	
		CheckerSocialEventRef event = static_pointer_cast<CheckerSocialEvent>(mEvent);	
		//console()<<"CheckerSocialEvent:::: "<<event->getSocialID()<<" ....  "<<event->getValue()<<endl;
		phbSettings->setSocialState(event->getSocialID(), event->getValue());
	}
	else if(typeid(*ev) == typeid(ChangePhotoOverDesignEvent))
	{
		ChangePhotoOverDesignEventRef event = static_pointer_cast<ChangePhotoOverDesignEvent>(mEvent);	
		//console()<<"PhotoTemplateChooseEvent:::: "<<event->getItem().getID()<<endl;
		phbSettings->setActiveOverDesignID(event->getItem().getID());
	}	
	else if(typeid(*ev) == typeid(ChangePhotoCardStyleDesignEvent))
	{
		ChangePhotoCardStyleDesignEventRef event = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(mEvent);	
		//console()<<"ChangePhotoCardStyleDesignEvent:::: "<<event->getItem().getID()<<endl;
		phbSettings->setActivePhotoCardStyleDesignID(event->getItem().getID());
	}
	else if(typeid(*ev) == typeid(ChangePhotoFilterPreviewActiveEvent))
	{
		ChangePhotoFilterPreviewActiveEventRef event = static_pointer_cast<ChangePhotoFilterPreviewActiveEvent>(mEvent);	
		//console()<<"ChangePhotoFilterPreviewActiveEvent:::: "<<event->getItem().getID()<<endl;
		phbSettings->swapFilter(event->getItem().getID());
	}	
	else if(typeid(*ev) == typeid(OpenSystemDirectoryEvent))
	{
		OpenSystemDirectoryEventRef event = static_pointer_cast<OpenSystemDirectoryEvent>(mEvent);	
		//console()<<"OpenSystemDirectoryEvent:::: "<<event->getPath()<<endl;
		fileTools().openSystemDirectory(event->getPath());			
	}
}

void PhotoboothConfig::activateListeners()
{
	for (auto layout : displayList)	
	{
		IPhotoboothItemRef ref = static_pointer_cast<IPhotoboothItem>(layout);
		ref->connectEventHandler(&PhotoboothConfig::mouseUpHandler, this);
		ref->activateTitleListeners();
	}	
}

void PhotoboothConfig::unActivateListeners()
{
	for (auto layout : displayList)	
	{
		layout->disconnectEventHandler();
		layout->unActivateListeners();
	}
}

void PhotoboothConfig::setOpenItem(int index)
{
	for (auto layout : displayList)		
	{
		IPhotoboothItemRef ref = static_pointer_cast<IPhotoboothItem>(layout);

		if (index == ref->getIndex())
			ref->activateListeners();	
		else
			ref->unActivateListeners();

		ref->setOpenLayoutIndex(index);	
	}				
}	

void PhotoboothConfig::showAnimate(const ci::EaseFn& eFunc, float time)
{	
	animatePosition = getGlobalPosition();
	unActivateListeners();

	timeline().apply( &animatePosition, ci::Vec2f(166.0f, 0.0f), time, eFunc)
		.finishFn(bind( &PhotoboothConfig::showAnimationFinish, this))
		.updateFn(bind( &PhotoboothConfig::animationPosUpdate, this));
}

void PhotoboothConfig::animationPosUpdate()
{
	setPosition(animatePosition.value());					
}

void PhotoboothConfig::showAnimationFinish()
{
	phbSettings->createMemento();
	activateListeners();
}

void PhotoboothConfig::hideAnimate(const ci::EaseFn& eFunc, float time)
{
	unActivateListeners();

	timeline().apply( &animatePosition, ci::Vec2f(1080.0f, 0.0f), time, eFunc)
		.finishFn(bind( &PhotoboothConfig::hideAnimationFinish, this))
		.updateFn(bind( &PhotoboothConfig::animationPosUpdate, this));
}		

void PhotoboothConfig::hideAnimationFinish()
{		
	setOpenItem(-1);
}