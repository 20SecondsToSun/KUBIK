#include "instagram/InstagramViewer.h"

using namespace instagram;
using namespace std;
using namespace ci;
using namespace ci::app;

InstagramViewer::InstagramViewer(InstagramClientRef client)
	:client(client),
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

	using namespace ci::app;
	mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&InstagramViewer::mouseDown, this, std::placeholders::_1));
	mouseUpCon	 = getWindow()->getSignalMouseUp().connect(std::bind(&InstagramViewer::mouseUp, this, std::placeholders::_1));
	mouseDragCon = getWindow()->getSignalMouseDrag().connect(std::bind(&InstagramViewer::mouseDrag, this, std::placeholders::_1));
	updateCon	 = App::get()->getSignalUpdate().connect(std::bind(&InstagramViewer::update, this));
	connected	 = true;
}

void InstagramViewer::disconnect()
{
	if (!connected) return;

	mouseDownCon.disconnect();
	mouseDragCon.disconnect();
	mouseUpCon.disconnect();
	updateCon.disconnect();
	connected = false;
}

void InstagramViewer::setPosition(float x, float y)
{
	initPosition = Vec2f(x, y);
}

void InstagramViewer::update()
{
	int plus = 0;
	auto images = client->getImages();
	if (images.size() % countInRaw != 0) plus = 1;
	mainHeight = (images.size() / countInRaw + plus) * (oneImageWidth + sdvigX);
}

void InstagramViewer::draw()
{
	gl::pushMatrices();
	gl::translate(currentPos.value());
	gl::translate(initPosition);

	int x = 0, y = 0;
	auto vec = client->getImages();

	for (size_t i = 0; i < vec.size(); i++)
	{
		x = (oneImageWidth + sdvigX) * (i % countInRaw);
		y = (oneImageWidth + sdvigY) * (i / countInRaw);
		vec[i].draw(Vec2f(x, y));
	}
	gl::popMatrices();
}

void InstagramViewer::mouseDown(MouseEvent event)
{
	if (event.getPos().y < initPosition.y) return;
	currentMousePos = event.getPos();
	wasDrag = false;
	downSecond = getElapsedSeconds();
}

void InstagramViewer::mouseUp(MouseEvent event)
{
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
	if (event.getPos().y < initPosition.y) return;

	wasDrag = true;

	delta = currentMousePos - event.getPos();

	if (blockDrag)
		return;

	futureCurrentPos -= Vec2i(0, delta.y);

	if (currentPos.value().y > marginTop)
	{
		futureCurrentPos = Vec2i::zero();
		blockDrag = true;
	}
	else if (getWindowHeight() - mainHeight - marginBottom > currentPos.value().y + initPosition.y)
	{
		futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight - initPosition.y);
		blockDrag = true;

		client->loadNextMedia();
	}

	timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
		.finishFn(bind(&InstagramViewer::animComplete, this));

	currentMousePos = event.getPos();
}

void InstagramViewer::getTouchedImage(const Vec2f& pos)
{
	int xCoord = (int)(pos.x / (oneImageWidth + sdvigX));
	int yCoord = (int)((pos.y - currentPos.value().y - initPosition.y) / (oneImageHeight + sdvigY));
	lastImageIndexTouched = yCoord * countInRaw + xCoord;

	if (lastImageIndexTouched < client->getImages().size())
		touchedEvent();
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
	client->clear();
}

int InstagramViewer::getLastImageIndexTouched()
{
	return lastImageIndexTouched;
}