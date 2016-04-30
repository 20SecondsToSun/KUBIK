#include "instagram/InstagramViewer.h"

using namespace instagram;
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

InstagramViewer::InstagramViewer(InstagramClientRef client,
	const gl::Texture& noMaterials,
	const gl::Texture& allLoaded,
	const gl::Texture& privateUser,
	const gl::Texture& notExistUser,
	const gl::Texture& notPhotosUser,
	const gl::Texture& dragToReload)
	:client(client),
	noMaterials(noMaterials),
	allLoaded(allLoaded),
	privateUser(privateUser),
	notExistUser(notExistUser),
	notPhotosUser(notPhotosUser),
	dragToReload(dragToReload),
	currentPos(Vec2f::zero()),
	futureCurrentPos(Vec2f::zero()),
	blockDrag(false),
	oneImageWidth(343),
	oneImageHeight(343),
	countInRaw(3),
	connected(false),
	mainHeight(0),
	marginBottom(300.0f),
	marginTop(400.0f),
	marginToShowUpdate(100.f),
	animTime(0.3f),
	animFunc(EaseOutQuad()),
	sdvigX(8),
	sdvigY(8),
	showAlphaDrag(true),
	AllLoaded(false)
{
	
}

void InstagramViewer::setDesignElements(
	const gl::Texture& noMaterials,
	const gl::Texture& allLoaded,
	const gl::Texture& privateUser,
	const gl::Texture& notExistUser,
	const gl::Texture& notPhotosUser,
	const gl::Texture& dragToReload)
{
	this->noMaterials = noMaterials;
	this->allLoaded = allLoaded;
	this->privateUser = privateUser;
	this->notExistUser = notExistUser;
	this->notPhotosUser = notPhotosUser;
	this->dragToReload = dragToReload;
}

void InstagramViewer::setPreloaderToneColor(const ci::ColorA& color)
{
	preloaderToneColor = color;
}

void InstagramViewer::connect()
{
	if (connected) return;

	mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&InstagramViewer::mouseDown, this, std::placeholders::_1));
	mouseUpCon	 = getWindow()->getSignalMouseUp().connect(std::bind(&InstagramViewer::mouseUp,		this, std::placeholders::_1));
	mouseDragCon = getWindow()->getSignalMouseDrag().connect(std::bind(&InstagramViewer::mouseDrag, this, std::placeholders::_1));
	connected	 = true;	
}

void InstagramViewer::disconnect()
{
	if (connected)
	{
		mouseDownCon.disconnect();
		mouseDragCon.disconnect();
		mouseUpCon.disconnect();
		connected = false;
	}
}

void InstagramViewer::showPreloader()
{
	setState(PRELOADING);
}

void InstagramViewer::setPosition(float x, float y)
{
	initPosition = Vec2f(x, y);
}

void InstagramViewer::animatePositionTo(float x, float y)
{
	timeline().apply(&initPosition, Vec2f(x, y), 0.2f, animFunc);
}

void InstagramViewer::synchImages()
{
	auto _newImages = client->getImages();
	images.insert(images.end(), _newImages.begin(), _newImages.end());	
	client->setSynch(false);

	if (images.empty())
	{
		setState(NO_MATERIALS);
		mainHeight = 0;
		return;
	}
	else if (images.size() > MAX_PHOTO_CASHED)
	{
		images.erase(images.begin(), images.begin() + CASH_PHOTO_CLEAR_COUNT);
		float correctionYPosition = (CASH_PHOTO_CLEAR_COUNT / countInRaw) * (oneImageWidth + sdvigX);
		currentPos.value().y += correctionYPosition;
		futureCurrentPos.y += correctionYPosition;
	}	

	if (images.size() < PHOTO_BLOCK_COUNT)
	{
		showingCount = images.size();
	}
	else
	{		
		if (_newImages.size()  < PHOTO_BLOCK_COUNT) // ok we are going to deep
		{
			showingCount = ((images.size() - _newImages.size()) / PHOTO_BLOCK_COUNT) * PHOTO_BLOCK_COUNT;
			showingCount += _newImages.size();
			AllLoaded = true;
		}	
		else
		{
			showingCount = (images.size() / PHOTO_BLOCK_COUNT) * PHOTO_BLOCK_COUNT;
		}
	}	

	if (showingCount % PHOTO_BLOCK_COUNT == 0)
	{
		mainHeight = (showingCount / countInRaw) * (oneImageWidth + sdvigX);
	}
	else
	{
		mainHeight = (showingCount / countInRaw + 1) * (oneImageWidth + sdvigX);
	}
	setState(IMAGES_DRAWING);			
}

void InstagramViewer::showPrivateUserState()
{
	setState(SHOW_PRIVATE_USER);
}

void InstagramViewer::showNotExistUser()
{
	setState(SHOW_NOT_EXIST_USER);
}

void InstagramViewer::showUserNotHavePhotos()
{
	setState(SHOW_USER_NOT_HAVE_PHOTOS);
}

void InstagramViewer::showNoHashtagPhotos()
{
	setState(SHOW_NO_HASHTAG_PHOTOS);
}

void InstagramViewer::showNoMoreImagesMsg()
{
	setState(SHOW_NO_MORE_POPUP);	
}

void InstagramViewer::noMorePopupAnimFinished()
{	
	setState(IMAGES_DRAWING);
}

void InstagramViewer::setState(const drawState& value)
{
	if (state == value)
		return;

	state = value;

	switch (state)
	{
	case SHOW_NO_MORE_POPUP:
		noMorePopupAlpha = 1.0f;
		timeline().apply(&noMorePopupAlpha, 1.0f, 0.0f, 0.8f, animFunc)
			.finishFn(bind(&InstagramViewer::noMorePopupAnimFinished, this)).delay(0.5f);
		break;

	case MINI_PRELOADING:
		preloaderMini = settingsFactory().getMiniPreloader();
		mainHeight += preloaderMini->getHeight() + 50;
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.value().y);
		currentPos.stop();
		timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
				.finishFn(bind(&InstagramViewer::animComplete, this));		
		break;

	case PRELOADING:		
		preloaderMain = settingsFactory().getMainPreloader();
		preloaderMain->setPosition(Vec2f((1080.0f - preloaderMain->getWidth()) * 0.5f, (1920.0f - preloaderMain->getHeight())*0.5f));
		break;
	}
}

void InstagramViewer::draw()
{
	switch (state)
	{
	case IMAGES_DRAWING:
		drawImages();
		break;

	case PRELOADING:
		drawMainPreloader();
		break;

	case MINI_PRELOADING:
		drawImages();
		drawMiniPreloader();
		break;	

	case SHOW_NO_MORE_POPUP:
		drawNoMorePopup();
		//drawImages();		
		break;

	case NO_MATERIALS:	
	case SHOW_NO_HASHTAG_PHOTOS:
		drawNoMaterialsPopup();
		break;

	case SHOW_PRIVATE_USER:
		drawPrivateUser();
		break;	

	case SHOW_NOT_EXIST_USER:
		drawNotExistUser();
		break;

	case SHOW_USER_NOT_HAVE_PHOTOS:
		drawUserNotHavePhotos();
		break;
	}	
}

void InstagramViewer::drawImages()
{
	gl::color(ColorA(1, 1, 1, alphaDragToReload));
	gl::draw(dragToReload, initPosition.value() + Vec2f((getWindowWidth() - dragToReload.getWidth()) * 0.5f, 20.0f));
	gl::color(Color::white());

	gl::pushMatrices();
	gl::translate(currentPos.value());
	gl::translate(initPosition);
	for (size_t i = 0; i < showingCount; i++)
	{
		int x = (oneImageWidth + sdvigX) * (i % countInRaw);
		int y = (oneImageWidth + sdvigY) * (i / countInRaw);
		images[i].draw(Vec2f(x, y));
	}
	gl::popMatrices();
}

void InstagramViewer::drawMainPreloader()
{
	preloaderMain->draw();
}

void InstagramViewer::drawMiniPreloader()
{
	gl::color(preloaderToneColor);
	gl::pushMatrices();
	gl::translate(currentPos.value());
	gl::translate(initPosition);
	gl::translate(0.0f, mainHeight - 150.0f);
	//gl::color(Color::hex(0xffffff));
	preloaderMini->draw();
	gl::popMatrices();
	gl::color(Color::white());
}

void InstagramViewer::drawNoMorePopup()
{
	gl::pushMatrices();
	gl::translate((getWindowWidth() - allLoaded.getWidth()) * 0.5f, getWindowHeight() - 40.0f);
	gl::draw(allLoaded);
	gl::popMatrices();
}

void InstagramViewer::drawNoMaterialsPopup()
{
	drawCenteredInfoImage(noMaterials, 522.0f);
}

void InstagramViewer::drawNotExistUser()
{
	drawCenteredInfoImage(notExistUser, 522.0f);
}

void InstagramViewer::drawUserNotHavePhotos()
{
	drawCenteredInfoImage(notPhotosUser, 522.0f);	
}

void InstagramViewer::drawPrivateUser()
{
	drawCenteredInfoImage(privateUser, 544.0f);	
}

void InstagramViewer::drawCenteredInfoImage(const ci::gl::Texture& image, float y)
{
	gl::pushMatrices();
	gl::translate(initPosition);
	gl::translate((getWindowWidth() - image.getWidth()) * 0.5f, y - image.getHeight() * 0.5f);
	gl::draw(image);
	gl::popMatrices();
}

void InstagramViewer::mouseDown(MouseEvent event)
{
	auto coordTransform = Utils::transformCoords(event.getPos());

	if (coordTransform.y < initPosition.value().y || images.empty()) return;

	if (images.size() < PHOTO_BLOCK_COUNT)
	{
		wasDrag = false;
		return;
	}

	currentMousePos = coordTransform;
	wasDrag = false;
	downSecond = getElapsedSeconds();
	touchedDownEvent();
}

void InstagramViewer::mouseUp(MouseEvent event)
{
	auto coordTransform = Utils::transformCoords(event.getPos());

	alphaDragToReload = 0.0f;

	if (images.empty())
	{
		return;
	}

	if (images.size() < PHOTO_BLOCK_COUNT)
	{
		if (!wasDrag && coordTransform.y > initPosition.value().y)
		{
			getTouchedImage(coordTransform);
		}
		return;
	}

	int bottomCorner = futureCurrentPos.y + mainHeight;

	if (futureCurrentPos.y > 0)
	{
		futureCurrentPos = Vec2i::zero();
		blockDrag = true;
		timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
			.finishFn(bind(&InstagramViewer::animComplete, this));
	}
	else if (bottomCorner < getWindowHeight() - initPosition.value().y)
	{
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.value().y);
		blockDrag = true;
		timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
			.finishFn(bind(&InstagramViewer::animComplete, this));
	}


	bool touchDeltaOk = ((delta.y >= -5 && delta.y <= 5) && ((getElapsedSeconds() - downSecond) < 0.19));
	if ((!wasDrag || touchDeltaOk) && coordTransform.y > initPosition.value().y)
	{
		getTouchedImage(coordTransform);
	}
}

void InstagramViewer::mouseDrag(MouseEvent event)
{
	auto coordTransform = Utils::transformCoords(event.getPos());
	
	if (coordTransform.y < initPosition.value().y || images.empty() || images.size() < PHOTO_BLOCK_COUNT) return;


	wasDrag = true;
	delta = currentMousePos - coordTransform;

	if (blockDrag)
		return;


	futureCurrentPos -= Vec2i(0, delta.y);

	alphaDragToReload = 0.0f;

	if (currentPos.value().y > marginTop)
	{
		futureCurrentPos = Vec2i::zero();
		blockDrag = true;
		AllLoaded = false;
		reloadAllMedia();
	}
	else if(currentPos.value().y > marginToShowUpdate && showAlphaDrag)
	{
		alphaDragToReload = ci::lmap((float)currentPos.value().y, marginToShowUpdate, marginToShowUpdate + 100, 0.0f, 1.0f);
	}	
	else if (getWindowHeight() - mainHeight - marginBottom > currentPos.value().y + initPosition.value().y && !AllLoaded)
	{
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.value().y);
		blockDrag = true;
		loadNextMedia();		
	}

	timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
		.finishFn(bind(&InstagramViewer::animComplete, this));

	currentMousePos = coordTransform;
}

void InstagramViewer::showMiniPreloader()
{
	setState(MINI_PRELOADING);
}

void InstagramViewer::getTouchedImage(const Vec2f& pos)
{
	int xCoord = (int)(pos.x / (oneImageWidth + sdvigX));
	int yCoord = (int)((pos.y - currentPos.value().y - initPosition.value().y) / (oneImageHeight + sdvigY));
	lastImageIndexTouched = yCoord * countInRaw + xCoord;

	if (!images.empty() && lastImageIndexTouched < images.size())
	{
		touchedEvent();
	}
}

void InstagramViewer::animComplete()
{
	blockDrag = false;
}

void InstagramViewer::clear()
{
	currentPos = futureCurrentPos = Vec2i::zero();
	blockDrag = false;	
	client->killLoad();
	images.clear();
}

int InstagramViewer::getLastImageIndexTouched()
{
	return lastImageIndexTouched;
}

ImageGraphic InstagramViewer::getImageGraphic()
{
	return images[lastImageIndexTouched];
}

void InstagramViewer::setTopDragVisible(bool value)
{
	showAlphaDrag = value;
}