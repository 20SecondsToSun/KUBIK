#include "SocialLocation.h"
#include "Tools/qrCode/QrCodeCreator.h"
#include "Server/Server.h"

using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::app;

SocialLocation::SocialLocation(ISettingsRef settings, StatCollectorRef statSettings, PhotoStorageRef  photoStorage)
	:IGameLocation(),
	statSettings(statSettings),
	photoStorage(photoStorage),
	startServiceButtonY(850.0f),
	leftBlockX(126.0f)
{
	reset(settings, statSettings);
}

void SocialLocation::reset(ISettingsRef settings, StatCollectorRef statsettings)
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Reset ~~~");

	removeAllChildren();
	IGameLocation::reset(settings);

	float rightBlockX = 652.0f;

	title = settings->getTexture("sharetitle");
	titlePos = Vec2f(rightBlockX, 203.0f - title.getHeight() * 0.5f);

	serviceBtns.clear();

	emailBtn   = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("email")));
	fbBtn	   = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("facebook")));
	vkBtn	   = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("vkontakte")));
	twBtn	   = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("twitter")));
	againBtn   = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("againbtn")));
	allAppBtn  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("othergamesbtn")));

	emailpopup = EmailPopupRef(new EmailPopup(settings, statsettings));
	vkpopup	   = VkontaktePopupRef(new VkontaktePopup(settings, statsettings));
	fbpopup	   = FacebookPopupRef(new FacebookPopup(settings, statsettings));
	twpopup	   = TwitterPopupRef(new TwitterPopup(settings, statsettings));

	emailpopup->setBackground(settings->getTexture("popupBg"));

	float shiftY = 238.0f;
	float posArrayWithEmail[] = { 468.0f, 776.0f, 776.0f + shiftY, 776.0f + 2.0f * shiftY };
	float posArrayWithoutEmail[] = { 468.0f, 468.0f + shiftY, 468.0f + 2.0f * shiftY };	
	float *currentY;

	auto settingspoza = static_pointer_cast<PozaSettings>(settings);

	if (settingspoza->getSocialState(PhtTextID::EMAIL))
	{
		currentY = &posArrayWithEmail[0];
		emailBtn->setPosition(Vec2f(rightBlockX, *currentY - emailBtn->getHeight() * 0.5f));
		addChild(emailBtn);
		serviceBtns.push_back(emailBtn);
		currentY++;
	}
	else
	{
		currentY = &posArrayWithoutEmail[0];
	}

	if (settingspoza->getSocialState(PhtTextID::FACEBOOK))
	{
		fbBtn->setPosition(Vec2f(rightBlockX, *currentY - fbBtn->getHeight() * 0.5f));
		addChild(fbBtn);
		serviceBtns.push_back(fbBtn);
		currentY++;
	}

	if (settingspoza->getSocialState(PhtTextID::VKONTAKTE))
	{
		vkBtn->setPosition(Vec2f(rightBlockX, *currentY - vkBtn->getHeight() * 0.5f));
		addChild(vkBtn);
		serviceBtns.push_back(vkBtn);
		currentY++;
	}

	if (settingspoza->getSocialState(PhtTextID::TWITTER))
	{
		twBtn->setPosition(Vec2f(rightBlockX, *currentY - twBtn->getHeight() * 0.5f));
		addChild(twBtn);
		serviceBtns.push_back(twBtn);
	}

	if (settingspoza->getSocialState(PhtTextID::QRCODE))
	{
		qrcode = QrCodeRef(new QrCode());
		auto qrTitle = settings->getTexture("qrtitle");
		qrcode->setTitle(qrTitle);
		qrcode->setPosition(Vec2f(leftBlockX, 880.0f - qrTitle.getHeight() * 0.5f));
	}
	else
	{
		qrCodeNullObject = QrCodeNullRef(new QrCodeNull());
		qrcode = qrCodeNullObject;
	}

	sharefon = settings->getTexture("sharefon");
	sharefonPos = Vec2f(0.0f, getWindowHeight() - sharefon.getHeight());

	//#ifdef Photobooth_Sharing_DEBUG
	startServiceButtonY = 588.0f;
	//#else
	startServiceButtonY = 1592.0f;
	//#endif

	againBtn->setPosition(Vec2f(127.0f, startServiceButtonY - againBtn->getHeight()  * 0.5f));
	allAppBtn->setPosition(Vec2f(581.0f, startServiceButtonY - allAppBtn->getHeight() * 0.5f));

	popup = emailpopup;
}

void SocialLocation::start()
{
	// set photos to post
	auto path1 = Paths::getPhotoTemplatePath().string(); 
	qrcode->clear();

	std::vector<std::string> filesPath;
	filesPath.push_back(Paths::getPhotoTemplateToServerPath(0).string());
	filesPath.push_back(Paths::getPhotoTemplateToServerPath(1).string());
	filesPath.push_back(Paths::getPhotoTemplateToServerPath(2).string());


	vkpopup->getSocialService()->setUploadPhotoPathVec(filesPath);
	fbpopup->getSocialService()->setUploadPhotoPathVec(filesPath);
	twpopup->getSocialService()->setUploadPhotoPathVec(filesPath);

	// set text to post
	vkpopup->getSocialService()->setPostingStatus(settings->getVKPostText());
	fbpopup->getSocialService()->setPostingStatus(settings->getFBPostText());
	twpopup->getSocialService()->setPostingStatus(settings->getTWPostText());

	connect_once(server().photoUploadSuccess, bind(&SocialLocation::photoUploadCompleteHandler, this, std::placeholders::_1, std::placeholders::_2));
	connect_once(server().photoUploadError, bind(&SocialLocation::photoUploadErrorHandler, this));

	uploadPhotoID = -1;
	try
	{
		server().postPhoto(Paths::getPhotoTemplateRibbonToServerPath().string(), toString(settings->getAppID()));
	}
	catch (...)
	{

	}
	//gifs

	//auto photoTemplate = settings->getPhotoSharingCard();
	//auto photoSticker = settings->getPhotoSharingSticker();

#ifndef Poza_Sharing_DEBUG
	finalPhotoTemplate.setData(photoStorage);
	//finalPhotoTemplate.setTemplate(photoTemplate);
	//finalPhotoTemplate.setSticker(photoSticker);
	finalPhotoTemplate.startAnimate();
#endif

	initShowAnim();
} 

void SocialLocation::photoUploadCompleteHandler(const std::string& photo_id, const std::string& link)
{
	//set qrcode
	uploadPhotoID = std::atoi(photo_id.c_str());
	logger().log(link);
	qrcode->initLink(link, settings->getQRCodeFile());
}

void SocialLocation::photoUploadErrorHandler()
{

}

void SocialLocation::initShowAnim()
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
	
	auto settingspoza = static_pointer_cast<PozaSettings>(settings);
	if (!settingspoza->onlyOneGameOn())
	{
		allAppBtn->showAnimate(0.0f, 1.0f, 0.8f, 0.4f);
		allAppBtn->showPositionAnimate(Vec2f(0.0f, 100.0f), 0.7f, 0.4f);
	}

	delaycall(bind(&SocialLocation::connectHandlers, this), showingTime + delay);
}

void SocialLocation::connectHandlers()
{
	againBtn->connectEventHandler(&SocialLocation::againBtnHandler, this);
	
	auto settingspoza = static_pointer_cast<PozaSettings>(settings);

	if (!settingspoza->onlyOneGameOn())
	{
		allAppBtn->connectEventHandler(&SocialLocation::allAppBtnHandler, this);
	}

	if (settingspoza->getSocialState(PhtTextID::EMAIL))
	{
		emailBtn->connectEventHandler(&SocialLocation::emailBtnHandler, this);
	}

	if (settingspoza->getSocialState(PhtTextID::FACEBOOK))
	{
		fbBtn->connectEventHandler(&SocialLocation::fbBtnHandler, this);
	}

	if (settingspoza->getSocialState(PhtTextID::VKONTAKTE))
	{
		vkBtn->connectEventHandler(&SocialLocation::vkBtnHandler, this);
	}

	if (settingspoza->getSocialState(PhtTextID::TWITTER))
	{
		twBtn->connectEventHandler(&SocialLocation::twBtnHandler, this);
	}

	callback(COMPLETE_ANIM);
}

void SocialLocation::againBtnHandler(EventGUIRef& event)
{
	disconnectEventHandlers();
	server().photoUploadSuccess.disconnect_all_slots();
	server().photoUploadError.disconnect_all_slots();
	setLastScreenShot();
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&SocialLocation::hideAnimComplete, this));
	state = ANIM_HIDE;
	callback(BEGIN_ANIM);
}

void SocialLocation::hideAnimComplete()
{
	callback(NEXT_LOC);
}

void SocialLocation::allAppBtnHandler(EventGUIRef& event)
{
	callback(CLOSE_LOCATION);
}

void SocialLocation::emailBtnHandler(EventGUIRef& event)
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Show Popup Email ~~~");
	//if (uploadPhotoID != -1)
	{
		emailpopup->setPhotoID(uploadPhotoID);
		popup = emailpopup;
		showPopup();
	}
}

void SocialLocation::fbBtnHandler(EventGUIRef& event)
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Show Popup Facebook ~~~");

	popup = fbpopup;
	showPopup();
}

void SocialLocation::vkBtnHandler(EventGUIRef& event)
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Show Popup Vkontakte ~~~");

	popup = vkpopup;
	showPopup();
}

void SocialLocation::twBtnHandler(EventGUIRef& event)
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Show Popup Twitter ~~~");

	popup = twpopup;
	showPopup();
}

void SocialLocation::stop()
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Stop ~~~");

	stopAllTweens();
	disconnectEventHandlers();
	finalPhotoTemplate.stopAnimate();
	clearDelaycall();
	popup->disconnectEventHandler(Popup::POPUP_CLOSED);
	popup->shareCompleteSignal.disconnect_all_slots();
	popup->kill();

	server().photoUploadSuccess.disconnect_all_slots();
	server().photoUploadError.disconnect_all_slots();
}

void SocialLocation::showPopup()
{
	callback(DISABLE_GAME_CLOSE);
	clearDelaycall();
	disconnectEventHandlers();
	finalPhotoTemplate.stopAnimate();
	setLastScreenShot();
	popup->connectEventHandler(&SocialLocation::popupClosed, this, Popup::POPUP_CLOSED);
	connect_once(popup->shareCompleteSignal, bind(&SocialLocation::shareCompleteHandler, this, std::placeholders::_1, std::placeholders::_2));
	popup->show();
	state = POPUP;
}

void SocialLocation::popupClosed()
{
	logger().log("~~~ Photobooth.SubLocation PhotoSharing.Popup Closed ~~~");

	callback(ENABLE_GAME_CLOSE);
	connectHandlers();
	state = TEMPLATE_CHOOSE;
	finalPhotoTemplate.startAnimate();
}

void SocialLocation::disconnectEventHandlers()
{
	auto settingspoza = static_pointer_cast<PozaSettings>(settings);

	againBtn->disconnectEventHandler();

	if (!settingspoza->onlyOneGameOn())
	{
		allAppBtn->disconnectEventHandler();
	}

	emailBtn->disconnectEventHandler();
	fbBtn->disconnectEventHandler();
	vkBtn->disconnectEventHandler();
	twBtn->disconnectEventHandler();
	popup->shareCompleteSignal.disconnect_all_slots();
}

void SocialLocation::update()
{

}

void SocialLocation::draw()
{
	fillBg();

	switch (state)
	{
	case SocialLocation::TEMPLATE_CHOOSE:
		gl::draw(sharefon, sharefonPos + sharefonPosAnim);
#ifndef Poza_Sharing_DEBUG
		drawFinalPhoto();
#endif
		qrcode->draw();
		drawTitle();
		drawServiceButtons();
		IGameLocation::draw();
		break;

	case SocialLocation::ANIM_HIDE:
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(screenshot);
		gl::color(Color::white());
		break;

	case SocialLocation::POPUP:
		gl::draw(screenshot);
		popup->draw();
		break;
	}
}

void SocialLocation::drawFinalPhoto()
{
	gl::pushMatrices();
	gl::translate(leftBlockX + leftBlockAnimateX, 132.0f);
	finalPhotoTemplate.draw();

	gl::popMatrices();
}

void SocialLocation::drawServiceButtons()
{
	auto settingspoza = static_pointer_cast<PozaSettings>(settings);

	againBtn->draw();

	if (!settingspoza->onlyOneGameOn())
	{
		allAppBtn->draw();
	}
}

void SocialLocation::stopAllTweens()
{
	leftBlockAnimateX.stop();
	sharefonPosAnim.stop();
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}

void SocialLocation::shareCompleteHandler(SharingType type, const std::string& linkToPost)
{
	logger().log("complete sharing :::  " + to_string(type));
	logger().log("linkToPost :::  " + linkToPost);

	switch (type)
	{
	case kubik::EMAIL:
		dbRecord->EmailShare++;
		break;

	case kubik::VK:
		dbRecord->VKhare++;
		if (dbRecord->VKLink.size())
		{
			dbRecord->VKLink += "," + linkToPost;
		}
		else
		{
			dbRecord->VKLink = linkToPost;
		}
		break;

	case kubik::FB:
		dbRecord->FBShare++;
		if (dbRecord->FBLink.size())
		{
			dbRecord->FBLink += "," + linkToPost;
		}
		else
		{
			dbRecord->FBLink = linkToPost;
		}
		break;

	case kubik::TW:
		dbRecord->TWShare++;
		if (dbRecord->TWLink.size())
		{
			dbRecord->TWLink += "," + linkToPost;
		}
		else
		{
			dbRecord->TWLink = linkToPost;
		}
		break;
	}
}
