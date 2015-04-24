#include "InstakubConfig.h"
using namespace kubik;
using namespace kubik::config;
using namespace ci;

InstakubConfig::InstakubConfig(InstakubSettingsRef instSettings):GameSettingsSprite(), instSettings(instSettings)			
{					
	searchBlock	   = SearchBlockRef(new SearchBlock(instSettings, Vec2i::zero()));
	hashTagBlock   = HashTagBlockRef(new HashTagBlock(instSettings, Vec2i(0, 435)));
	photoCardStyle = InstaPhotoCardStyleRef(new InstaPhotoCardStyle(instSettings, Vec2i(0, 920)));//920
	keyBackground = KeyBackgroundRef(new KeyBackground());
	popup = InstaErrorPopupRef(new InstaErrorPopup(instSettings->getTexture("errorText")));
	
	addChild(searchBlock);
	addChild(hashTagBlock);
	addChild(photoCardStyle);
	addChild(keyBackground);
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
		console() << "OpenSystemDirectoryEvent:::: "<< _event->getPath() << endl;
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
	//clearDelaycall();
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
	touchKeyboard().hide(Vec2f(30.0f, 1920.0f), 0.3f);
	keyBackground->close();
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
}		

void InstakubConfig::hideAnimationFinish()
{		

}

void InstakubConfig::initKeyBoard()
{
	touchKeyboard().setDefaultSettings();
	touchKeyboard().hideQuick(Vec2f(-130.0f, 1920.0f));
	touchKeyboard().setOriginPoint(Vec2f(166.0f, 0.0f));
	touchKeyboard().setInputField(162.0f, 695.0f, 808.0f, 797.0f);
	touchKeyboard().setInputFont(instSettings->getFont("introLight44"));
	touchKeyboard().setInputColor(Color::black());
	touchKeyboard().setInputFieldText(instSettings->getHashtag());
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&InstakubConfig::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);				
}

void InstakubConfig::inputTouchHandler()
{
	float time = 0.7f;
	photoCardStyle->unActivateListeners();
	touchKeyboard().show(Vec2f(-130.0f, 1920.0f), Vec2f(-130.0f, 1010.0f), time);
	
	keyBackground->show(EaseOutCubic(), time);
	keyBackground->connectEventHandler(&InstakubConfig::closeKeyboardHandler, this, KeyBackground::HIDE_KEYBOARD);	
}

void InstakubConfig::closeKeyboardHandler()
{
	touchKeyboard().hide(Vec2f(-130.0f, 1920.0f), 0.3f);
	keyBackground->hide(EaseOutCubic(), 0.7f);
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	photoCardStyle->activateListeners();
}

void InstakubConfig::keyBoardShowComplete()
{

}

void InstakubConfig::draw()
{
	Sprite::draw();

	gl::pushMatrices();				
	gl::translate(getGlobalPosition());
	touchKeyboard().draw();
	gl::popMatrices();

	popup->draw();
}

void InstakubConfig::showErrorPopup()
{
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);

	popup->show(EaseOutCubic(), 0.7f);
	popup->connectEventHandler(&InstakubConfig::popupClosed, this, InstaErrorPopup::HIDE_POPUP);

	unActivateListeners();
}

void InstakubConfig::popupClosed()
{
	popup->hide(EaseOutCubic(), 0.7f);
	popup->disconnectEventHandler(InstaErrorPopup::HIDE_POPUP);
	activateListeners();
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&InstakubConfig::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);

}

bool InstakubConfig::canClose()
{
	return touchKeyboard().getInputFieldText() != "" || searchBlock->isChecked();
}