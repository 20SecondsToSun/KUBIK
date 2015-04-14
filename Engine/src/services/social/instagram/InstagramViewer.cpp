#include "instagram/InstagramViewer.h"

using namespace instagram;
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;

InstagramViewer::InstagramViewer(InstagramClientRef client, 
	ImageSequencerRef preloaderMain,
	const gl::Texture& preloaderMini,
	const gl::Texture& noMaterials,
	const gl::Texture& allLoaded)
	:client(client),
	preloaderMain(preloaderMain),
	preloaderMini(preloaderMini),
	noMaterials(noMaterials),
	allLoaded(allLoaded),
	currentPos(Vec2f::zero()),
	futureCurrentPos(Vec2f::zero()),
	blockDrag(false),
	oneImageWidth(343),
	oneImageHeight(343),
	countInRaw(3),
	connected(false),
	mainHeight(0),
	marginBottom(400),
	marginTop(400),
	animTime(0.3f),
	animFunc(EaseOutQuad()),
	sdvigX(8),
	sdvigY(8)
{
	
}

void InstagramViewer::connect()
{
	if (connected) return;

	mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&InstagramViewer::mouseDown, this, std::placeholders::_1));
	mouseUpCon	 = getWindow()->getSignalMouseUp().connect(std::bind(&InstagramViewer::mouseUp, this, std::placeholders::_1));
	mouseDragCon = getWindow()->getSignalMouseDrag().connect(std::bind(&InstagramViewer::mouseDrag, this, std::placeholders::_1));
	connected	 = true;	
}

void InstagramViewer::disconnect()
{
	if (!connected) return;

	mouseDownCon.disconnect();
	mouseDragCon.disconnect();
	mouseUpCon.disconnect();
	connected = false;
}

void InstagramViewer::showPreloader()
{
	setState(PRELOADING);
}

void InstagramViewer::setPosition(float x, float y)
{
	initPosition = Vec2f(x, y);
}

void InstagramViewer::synchImages()
{
	auto _newImages = client->getImages();
	images.insert(images.end(), _newImages.begin(), _newImages.end());
	client->setSynch(false);

	int plus = 0;
	if (images.size() % countInRaw != 0) plus = 1;

	mainHeight = (images.size() / countInRaw + plus) * (oneImageWidth + sdvigX);

	if (!images.empty())	
		setState(IMAGES_DRAWING);	
	else
		setState(NO_MATERIALS);
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
	case PRELOADING:
		//if (!preloaderMain.isPlaying())
		{
			
		//	preloaderMain.setLoop();
			//preloaderMain.play();
		}
		return;

	case SHOW_NO_MORE_POPUP:
		noMorePopupAlpha = 1.0f;
		timeline().apply(&noMorePopupAlpha, 1.0f, 0.0f, 0.8f, animFunc)
			.finishFn(bind(&InstagramViewer::noMorePopupAnimFinished, this)).delay(0.5);
		break;	
	}

	//if (preloaderMain.isPlaying())	
	//	preloaderMain.stop();	
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
		drawImages();
		drawNoMorePopup();
		break;

	case NO_MATERIALS:		
		drawNoMaterialsPopup();
		break;
	}	
}

void InstagramViewer::drawImages()
{
	gl::pushMatrices();
	gl::translate(currentPos.value());
	gl::translate(initPosition);

	int x = 0, y = 0;

	for (size_t i = 0; i < images.size(); i++)
	{
		x = (oneImageWidth + sdvigX) * (i % countInRaw);
		y = (oneImageWidth + sdvigY) * (i / countInRaw);
		images[i].draw(Vec2f(x, y));
	}
	gl::popMatrices();
}

void InstagramViewer::drawMainPreloader()
{
	gl::pushMatrices();	
	gl::translate(initPosition);
	gl::translate(0, 100);
	preloaderMain->draw();
	gl::popMatrices();
}

void InstagramViewer::drawMiniPreloader()
{
	gl::pushMatrices();
	gl::translate(currentPos.value());
	gl::translate(initPosition);
	gl::translate((getWindowWidth() - preloaderMini.getWidth())*0.5f, mainHeight);
	gl::draw(preloaderMini);
	gl::popMatrices();
}

void InstagramViewer::drawNoMorePopup()
{
	gl::pushMatrices();
	gl::translate(initPosition);
	gl::translate((getWindowWidth() - allLoaded.getWidth()) * 0.5f, 100);
	gl::color(ColorA(1, 1, 1, noMorePopupAlpha));
	gl::draw(allLoaded);
	gl::popMatrices();
}

void InstagramViewer::drawNoMaterialsPopup()
{
	gl::pushMatrices();
	gl::translate(initPosition);
	gl::translate((getWindowWidth() - noMaterials.getWidth()) * 0.5f, 100);	
	gl::draw(noMaterials);
	gl::popMatrices();
}

void InstagramViewer::mouseDown(MouseEvent event)
{
	if (event.getPos().y < initPosition.y || images.empty()) return;
	currentMousePos = event.getPos();
	wasDrag = false;
	downSecond = getElapsedSeconds();
}

void InstagramViewer::mouseUp(MouseEvent event)
{
	if (images.empty())
		return;

	int bottomCorner = futureCurrentPos.y + mainHeight;

	if (futureCurrentPos.y > 0)
	{
		futureCurrentPos = Vec2i::zero();
		blockDrag = true;
		timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
			.finishFn(bind(&InstagramViewer::animComplete, this));
	}
	else if (bottomCorner < getWindowHeight() - initPosition.y)
	{
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.y);
		blockDrag = true;
		timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
			.finishFn(bind(&InstagramViewer::animComplete, this));
	}

	if (!wasDrag && event.getPos().y > initPosition.y)
		getTouchedImage(event.getPos());
}

void InstagramViewer::mouseDrag(MouseEvent event)
{
	if (event.getPos().y < initPosition.y || images.empty()) return;

	wasDrag = true;

	delta = currentMousePos - event.getPos();

	if (blockDrag)
		return;

	futureCurrentPos -= Vec2i(0, delta.y);

	if (currentPos.value().y > marginTop)
	{
		futureCurrentPos = Vec2i::zero();
		blockDrag = true;
		reloadAllMedia();
	}
	else if (getWindowHeight() - mainHeight - marginBottom > currentPos.value().y + initPosition.y)
	{
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.y);
		blockDrag = true;
		loadNextMedia();		
	}

	timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
		.finishFn(bind(&InstagramViewer::animComplete, this));

	currentMousePos = event.getPos();
}

void InstagramViewer::showMiniPreloader()
{
	setState(MINI_PRELOADING);
}

void InstagramViewer::getTouchedImage(const Vec2f& pos)
{
	int xCoord = (int)(pos.x / (oneImageWidth + sdvigX));
	int yCoord = (int)((pos.y - currentPos.value().y - initPosition.y) / (oneImageHeight + sdvigY));
	lastImageIndexTouched = yCoord * countInRaw + xCoord;

	if (!images.empty() && lastImageIndexTouched < images.size())
		touchedEvent();
}

ImageGraphic InstagramViewer::getImageGraphic()
{
	return images[lastImageIndexTouched];
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