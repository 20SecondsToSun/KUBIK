#include "InstakubConfig.h"
using namespace kubik;
using namespace kubik::config;
using namespace ci;

InstakubConfig::InstakubConfig(InstakubSettingsRef instSettings):GameSettingsSprite(), instSettings(instSettings)			
{					
	searchBlock	   = SearchBlockRef(new SearchBlock(instSettings, Vec2i(0.0f, 0.0f)));
	hashTagBlock   = HashTagBlockRef(new HashTagBlock(instSettings, Vec2i(0.0f, 435.0f)));
	photoCardStyle = InstaPhotoCardStyleRef(new InstaPhotoCardStyle(instSettings, Vec2i(0.0f, 920.0f)));//920

	addChild(searchBlock);
	addChild(hashTagBlock);
	addChild(photoCardStyle);	
}	

void InstakubConfig::writeConfig()
{
	instSettings->writeConfig();
}

void InstakubConfig::mouseUpHandler(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if(!ev)	return;

	if(typeid(*ev) == typeid(ChangePhotoCardStyleDesignEvent))
	{		
		ChangePhotoCardStyleDesignEventRef _event = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(event);	
		int id = _event->getItem().getID();		
		instSettings->setActivePhotoCardStyleDesignID(id);
		console()<<"ChangePhotoCardStyleDesignEvent  "<<id<<endl;
	}
	else if(typeid(*ev) == typeid(OpenSystemDirectoryEvent))
	{		
		OpenSystemDirectoryEventRef _event = static_pointer_cast<OpenSystemDirectoryEvent>(event);	
		fileTools().openSystemDirectory(_event->getPath());		
	}
	else if(typeid(*ev) == typeid(HashCheckerEvent))
	{
		HashCheckerEventRef _event = static_pointer_cast<HashCheckerEvent>(event);	
		instSettings->setSearchFlag(_event->getValue());
		console()<<"HashCheckerEvent---------->  "<<_event->getValue()<<endl;
	}	
}

void InstakubConfig::activateListeners()
{
	for (auto layout : displayList)	
	{
		layout->connectEventHandler(&InstakubConfig::mouseUpHandler, this);
		layout->activateListeners();
	}	
}

void InstakubConfig::unActivateListeners()
{
	for (auto layout : displayList)	
	{
		layout->disconnectEventHandler();
		layout->unActivateListeners();
	}
}			

void InstakubConfig::showAnimate(const EaseFn& eFunc, float time) 
{	
	unActivateListeners();
	initKeyBoard();

	animatePosition = getGlobalPosition();				
	timeline().apply( &animatePosition, Vec2f(166.0f, 0.0f), time, eFunc)
		.finishFn(bind( &InstakubConfig::showAnimationFinish, this))
		.updateFn(bind( &InstakubConfig::animationPosUpdate, this));	
}

void InstakubConfig::animationPosUpdate()
{
	setPosition(animatePosition.value());					
}

void InstakubConfig::showAnimationFinish()
{
	instSettings->createMemento();
	activateListeners();
}

void InstakubConfig::hideAnimate(const EaseFn& eFunc, float time)
{
	unActivateListeners();

	timeline().apply( &animatePosition, Vec2f(1080.0f, 0.0f), time, eFunc)
		.finishFn(bind( &InstakubConfig::hideAnimationFinish, this))
		.updateFn(bind( &InstakubConfig::animationPosUpdate, this));

	
	instSettings->setHashtag(touchKeyboard().getInputFieldText());

	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);	
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hide(Vec2f(30.0f, 1200.0f), 0.3f);
}		

void InstakubConfig::hideAnimationFinish()
{		

}

void InstakubConfig::initKeyBoard()
{	
	touchKeyboard().setInputFieldText(instSettings->getHashtag());
	touchKeyboard().setOriginPoint(Vec2f(166.0f, 0.0f));
	touchKeyboard().setInputField(159.0f, 695.0f, 809.0f, 797.0f);
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&InstakubConfig::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);				
}

void InstakubConfig::inputTouchHandler()
{
	touchKeyboard().show(Vec2f(-130.0f, 800.0f), Vec2f(-130.0f, 760.0f), 0.7f);
}

void InstakubConfig::draw()
{
	Sprite::draw();

	gl::pushMatrices();				
	gl::translate(getGlobalPosition());
	touchKeyboard().draw();
	gl::popMatrices();				
}