#include "PhotoSharing.h"

using namespace kubik::games::photobooth;
using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::app;

#define debug

PhotoSharing::PhotoSharing(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage)
	:IPhotoboothLocation(),
	photoStorage(photoStorage),
	startServiceButtonY(850.0f),
	leftBlockX(126.0f)
{
	reset(settings);
};

void PhotoSharing::reset(PhotoboothSettingsRef settings)
{
	removeAllChildren();
	IPhotoboothLocation::reset(settings);

	float rightBlockX = 652.0f;
	title = settings->getTexture("sharetitle");
	titlePos = Vec2f(rightBlockX, 203.0f - title.getHeight() * 0.5f);

	serviceBtns.clear();

	emailBtn  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("email")));
	fbBtn	  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("facebook")));
	vkBtn	  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("vkontakte")));
	twBtn	  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("twitter")));
	againBtn  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("again")));
	allAppBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("allApp")));

	emailpopup = EmailPopupRef(new EmailPopup(settings));
	vkpopup = VkontaktePopupRef(new VkontaktePopup(settings));
	fbpopup = FacebookPopupRef(new FacebookPopup(settings));
	twpopup = TwitterPopupRef(new TwitterPopup(settings));

	emailpopup->setBackground(settings->getTexture("popupBg"));
	
	float shiftY = 238.0f;
	float posArrayWithEmail[]	 = { 468.0f, 776.0f, 776.0f + shiftY, 776.0f + 2.0f * shiftY };
	float posArrayWithoutEmail[] = { 468.0f, 468.0f + shiftY, 468.0f + 2.0f * shiftY };
	float *currentY;

	if (settings->getSocialState(PhtTextID::EMAIL))
	{
		currentY = &posArrayWithEmail[0];
		emailBtn->setPosition(Vec2f(rightBlockX, *currentY - emailBtn->getHeight() * 0.5f));
		addChild(emailBtn);
		serviceBtns.push_back(emailBtn);
		currentY++;
	}
	else	
		currentY = &posArrayWithoutEmail[0];	

	if (settings->getSocialState(PhtTextID::FACEBOOK))
	{
		fbBtn->setPosition(Vec2f(rightBlockX, *currentY - fbBtn->getHeight() * 0.5f));
		addChild(fbBtn);
		serviceBtns.push_back(fbBtn);
		currentY++;
	}

	if (settings->getSocialState(PhtTextID::VKONTAKTE))
	{
		vkBtn->setPosition(Vec2f(rightBlockX, *currentY - vkBtn->getHeight() * 0.5f));
		addChild(vkBtn);
		serviceBtns.push_back(vkBtn);
		currentY++;
	}

	if (settings->getSocialState(PhtTextID::TWITTER))
	{
		twBtn->setPosition(Vec2f(rightBlockX, *currentY - twBtn->getHeight() * 0.5f));
		addChild(twBtn);
		serviceBtns.push_back(twBtn);
	}

	if (settings->getSocialState(PhtTextID::QRCODE))
	{
		qrcode = QrCodeRef(new QrCode());
		auto qrTitle = settings->getTexture("qrtitle");
		qrcode->setTtile(qrTitle);
		qrcode->setPosition(Vec2f(leftBlockX, 880.0f - qrTitle.getHeight() * 0.5f));
	}
	else
	{
		qrCodeNullObject = QrCodeNullRef(new QrCodeNull());
		qrcode = qrCodeNullObject;
	}

	////////////////////////////////////////////

	sharefon = settings->getTexture("sharefon");
	sharefonPos = Vec2f(0.0f, getWindowHeight() - sharefon.getHeight());
	startServiceButtonY = 588.0f;

	againBtn->setPosition(Vec2f(127.0f, startServiceButtonY  - againBtn->getHeight()  * 0.5f));
	allAppBtn->setPosition(Vec2f(581.0f, startServiceButtonY - allAppBtn->getHeight() * 0.5f));

	popup = emailpopup;
}

void PhotoSharing::start()
{
	console() << "start PhotoSharing" << endl;

	auto photoTemplate = settings->getPhotoCardStylesActiveTemplate()[1];

	#ifndef debug
	finalPhotoTemplate.setData(photoStorage);
	finalPhotoTemplate.setTemplate(photoTemplate);
	finalPhotoTemplate.startAnimate();
	#endif

	qrcode->initLink();
	initShowAnim();
}

void PhotoSharing::initShowAnim()
{
	state = TEMPLATE_CHOOSE;

	titleAlpha = 0.0f;
	leftBlockAnimateX = -leftBlockX;
	sharefonPosAnim = Vec2f(0.0f, sharefon.getHeight());
	qrcode->setAlpha(0.0f);

	timeline().apply(&leftBlockAnimateX, 0.0f, 0.4f, EaseOutCubic());
	timeline().apply(&titleAlpha, 1.0f, 0.4f, EaseOutCubic());
	timeline().apply(&sharefonPosAnim, Vec2f::zero(), 0.4f, EaseOutCubic());

	float delay = 0.0f, showingTime = 0.8f;
	for (size_t i = 0; i < serviceBtns.size(); i++, delay += 0.1f)
	{
		serviceBtns[i]->showAnimate(0.0f, 1.0f, 0.8f, delay);
		serviceBtns[i]->showPositionAnimate(Vec2f(100, 0.0f), 0.8f, delay);
	}

	titleAnimPosition = titlePos + Vec2f(200.0f, 0.0f);
	timeline().apply(&titleAnimPosition, titlePos, 0.5f, EaseOutCubic());

	qrcode->showAnimate(0.0f, 1.0f, 0.8f, 0.2f);

	againBtn->showAnimate(0.0f, 1.0f, 0.8f, 0.3f);
	againBtn->showPositionAnimate(Vec2f(0.0f, 100.0f), 0.7f, 0.3f);

	allAppBtn->showAnimate(0.0f, 1.0f, 0.8f, 0.4f);
	allAppBtn->showPositionAnimate(Vec2f(0.0f, 100.0f), 0.7f, 0.4f);

	delaycall(bind(&PhotoSharing::connectHandlers, this), showingTime + delay);
}

void PhotoSharing::connectHandlers()
{
	console() << "CONNECT HANDLERS:::::::::::::::::: " << endl;
	againBtn->connectEventHandler(&PhotoSharing::againBtnHandler, this);
	allAppBtn->connectEventHandler(&PhotoSharing::allAppBtnHandler, this);

	if (settings->getSocialState(PhtTextID::EMAIL))	
		emailBtn->connectEventHandler(&PhotoSharing::emailBtnHandler, this);

	if (settings->getSocialState(PhtTextID::FACEBOOK))
		fbBtn->connectEventHandler(&PhotoSharing::fbBtnHandler, this);

	if (settings->getSocialState(PhtTextID::VKONTAKTE))
		vkBtn->connectEventHandler(&PhotoSharing::vkBtnHandler, this);

	if (settings->getSocialState(PhtTextID::TWITTER))
		twBtn->connectEventHandler(&PhotoSharing::twBtnHandler, this);

	callback(COMPLETE_ANIM);
}

void PhotoSharing::againBtnHandler(EventGUIRef& event)
{
	console() << "againBtnHandler" << endl;
	disconnectEventHandlers();
	setLastScreenShot();
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&PhotoSharing::hideAnimComplete, this));
	state = ANIM_HIDE;
	callback(BEGIN_ANIM);
}

void PhotoSharing::hideAnimComplete()
{
	callback(NEXT_LOC);
}

void PhotoSharing::allAppBtnHandler(EventGUIRef& _event)
{
	console() << "allAppBtnHandler" << endl;
}

void PhotoSharing::emailBtnHandler(EventGUIRef& _event)
{
	console() << "emailBtnHandler" << endl;
	popup = emailpopup;
	showPopup();
}

void PhotoSharing::fbBtnHandler(EventGUIRef& _event)
{
	console() << "fbBtnHandler" << endl;
	popup = fbpopup;
	showPopup();
}

void PhotoSharing::vkBtnHandler(EventGUIRef& _event)
{
	console() << "vkBtnHandler" << endl;
	popup = vkpopup;
	showPopup();
}

void PhotoSharing::twBtnHandler(EventGUIRef& _event)
{
	console() << "twBtnHandler" << endl;
	popup = twpopup;
	showPopup();
}

void PhotoSharing::stop()
{
	console() << "stop PhotoSharing" << endl;
	stopAllTweens();
	disconnectEventHandlers();
	finalPhotoTemplate.stopAnimate();
	clearDelaycall();	
	popup->disconnectEventHandler(Popup::POPUP_CLOSED);
	popup->kill();
}

void PhotoSharing::showPopup()
{
	callback(DISABLE_GAME_CLOSE);
	clearDelaycall();
	disconnectEventHandlers();
	finalPhotoTemplate.stopAnimate();
	setLastScreenShot();
	popup->connectEventHandler(&PhotoSharing::popupClosed, this, Popup::POPUP_CLOSED);
	popup->show();	
	state = POPUP;
}

void PhotoSharing::popupClosed()
{
	callback(ENABLE_GAME_CLOSE);
	connectHandlers();
	state = TEMPLATE_CHOOSE;
}

void PhotoSharing::disconnectEventHandlers()
{
	console() << "DISCONNECT HANDLERS:::::::::::::::::: " << endl;
	againBtn->disconnectEventHandler();
	allAppBtn->disconnectEventHandler();
	emailBtn->disconnectEventHandler();
	fbBtn->disconnectEventHandler();
	vkBtn->disconnectEventHandler();
	twBtn->disconnectEventHandler();
}

void PhotoSharing::update()
{

}

void PhotoSharing::draw()
{
	fillBg();
	
	switch (state)
	{
	case PhotoSharing::TEMPLATE_CHOOSE:
		gl::draw(sharefon, sharefonPos + sharefonPosAnim);
#ifndef debug
		drawFinalPhoto();
#endif
		qrcode->draw();
		drawTitle();
		drawServiceButtons();
		IPhotoboothLocation::draw();
		break;

	case PhotoSharing::ANIM_HIDE:
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(screenshot);
		gl::color(Color::white());
		break;

	case PhotoSharing::POPUP:
		gl::draw(screenshot);
		popup->draw();
		break;
	}
}

void PhotoSharing::drawFinalPhoto()
{
	gl::pushMatrices();
	gl::translate(leftBlockX + leftBlockAnimateX, 132.0f);
	finalPhotoTemplate.draw();
	gl::popMatrices();
}

void PhotoSharing::drawServiceButtons()
{
	againBtn->draw();
	allAppBtn->draw();
}

void PhotoSharing::stopAllTweens()
{
	leftBlockAnimateX.stop();
	sharefonPosAnim.stop();
	alphaAnim.stop();
	IPhotoboothLocation::stopAllTweens();
}