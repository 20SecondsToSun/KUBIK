#include "PhotoSharing.h"

using namespace kubik::games::photobooth;
using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::app;

PhotoSharing::PhotoSharing(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage):photoStorage(photoStorage),
	startServiceButtonY(850)
{			
	reset(settings);		
};

void PhotoSharing::reset(PhotoboothSettingsRef settings)
{
	removeAllChildren();
	IPhotoboothLocation::reset(settings);
	
	float rightBlockX = 652.0f;
	texTitle	 = settings->getTexture("sharetitle");
	texTitlePos	 = Vec2f(rightBlockX, 203.0f - texTitle.getHeight() * 0.5);	

	leftBlockX = 126.0f;
	qrCodeFlag = settings->getSocialState(PhtTextID::QRCODE);
	if(qrCodeFlag)
	{
		qrTitle	 = settings->getTexture("qrtitle");
		qrTitlePos = Vec2f(leftBlockX, 880.0f - qrTitle.getHeight() * 0.5);
	}	

	emailBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("email")));
	fbBtn	 = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("facebook")));
	vkBtn	 = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("vkontakte")));
	twBtn	 = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("twitter")));

	float shiftY = 238.0f;
	float posArrayWithEmail[] = {468, 776, 776 + shiftY, 776 + 2*shiftY};	
	float posArrayWithoutEmail[] = {468, 468 + shiftY, 468 + 2*shiftY};	
	float *currentY;

	if(settings->getSocialState(PhtTextID::EMAIL))
	{
		currentY = &posArrayWithEmail[0];
		emailBtn->setPosition(Vec2f(rightBlockX, *currentY - emailBtn->getHeight() * 0.5f));
		addChild(emailBtn);
		currentY++;
	}
	else
	{
		currentY = &posArrayWithoutEmail[0];
	}

	if(settings->getSocialState(PhtTextID::FACEBOOK))
	{
		fbBtn->setPosition(Vec2f(rightBlockX, *currentY - fbBtn->getHeight() * 0.5f));
		addChild(fbBtn);
		currentY++;
	}

	if(settings->getSocialState(PhtTextID::VKONTAKTE))
	{
		vkBtn->setPosition(Vec2f(rightBlockX, *currentY - vkBtn->getHeight() * 0.5f));
		addChild(vkBtn);
		currentY++;
	}

	if(settings->getSocialState(PhtTextID::TWITTER))
	{
		twBtn->setPosition(Vec2f(rightBlockX, *currentY - twBtn->getHeight() * 0.5f));
		addChild(twBtn);		
	}

	////////////////////////////////////////////

	sharefon  = settings->getTexture("sharefon");
	sharefonPos = Vec2f(0.0f, getWindowHeight() - sharefon.getHeight());

	againBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("again")));
	allAppBtn= ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("allApp")));

	startServiceButtonY = 1100;//1588
	againBtn->setPosition(Vec2f(127, startServiceButtonY - againBtn->getHeight() * 0.5f));
	startServiceButtonY = 1100;//1588
	allAppBtn->setPosition(Vec2f(581, startServiceButtonY - allAppBtn->getHeight() * 0.5f));
}		

void PhotoSharing::start()
{
	console()<<"start PhotoSharing"<<endl;

	auto photoTemplate = settings->getPhotoCardStylesActiveTemplate()[1];
	
	finalPhotoTemplate.setData(photoStorage);
	finalPhotoTemplate.setTemplate(photoTemplate);
	finalPhotoTemplate.startAnimate();


	againBtn->connectEventHandler(&PhotoSharing::againBtnHandler, this);	
	allAppBtn->connectEventHandler(&PhotoSharing::allAppBtnHandler, this);	

	if(settings->getSocialState(PhtTextID::EMAIL))
		emailBtn->connectEventHandler(&PhotoSharing::emailBtnHandler, this);

	if(settings->getSocialState(PhtTextID::FACEBOOK))
		fbBtn->connectEventHandler(&PhotoSharing::fbBtnHandler, this);	

	if(settings->getSocialState(PhtTextID::VKONTAKTE))
		vkBtn->connectEventHandler(&PhotoSharing::vkBtnHandler, this);	

	if(settings->getSocialState(PhtTextID::TWITTER))
		twBtn->connectEventHandler(&PhotoSharing::twBtnHandler, this);	
}

void PhotoSharing::againBtnHandler(EventGUIRef& _event)
{
	console()<<"againBtnHandler"<<endl;
}

void PhotoSharing::allAppBtnHandler(EventGUIRef& _event)
{
	console()<<"allAppBtnHandler"<<endl;
}

void PhotoSharing::emailBtnHandler(EventGUIRef& _event)
{
	console()<<"emailBtnHandler"<<endl;
}

void PhotoSharing::fbBtnHandler(EventGUIRef& _event)
{
	console()<<"fbBtnHandler"<<endl;
}

void PhotoSharing::vkBtnHandler(EventGUIRef& _event)
{
	console()<<"vkBtnHandler"<<endl;
}

void PhotoSharing::twBtnHandler(EventGUIRef& _event)
{
	console()<<"twBtnHandler"<<endl;
}

void PhotoSharing::stop()
{
	console()<<"stop PhotoSharing"<<endl;

	againBtn->disconnectEventHandler();
	allAppBtn->disconnectEventHandler();
	emailBtn->disconnectEventHandler();	
	fbBtn->disconnectEventHandler();
	vkBtn->disconnectEventHandler();	
	twBtn->disconnectEventHandler();
	finalPhotoTemplate.stopAnimate();
}

void PhotoSharing::update() 
{

}

void PhotoSharing::draw() 
{
	fillBg();
	gl::draw(sharefon, sharefonPos);
	drawFinalPhoto();

	if(qrCodeFlag)	
		gl::draw(qrTitle, qrTitlePos);	

	gl::draw(texTitle, texTitlePos);
	drawServiceButtons();
	IPhotoboothLocation::draw();
}

void PhotoSharing::drawFinalPhoto() 
{
	gl::pushMatrices();	
	gl::translate(leftBlockX, 132.0f);	
	finalPhotoTemplate.draw();		
	gl::popMatrices();
}

void PhotoSharing::drawServiceButtons() 
{
	
	againBtn->draw();
	allAppBtn->draw();
}