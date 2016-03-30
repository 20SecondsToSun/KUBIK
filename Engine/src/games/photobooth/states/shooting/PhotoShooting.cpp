#include "PhotoShooting.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace shaders::imagefilters;

PhotoShooting::PhotoShooting(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage)
	:photoStorage(photoStorage),
	progressBlockStartY(1688.0f),
	delayShootingTime(3.0f),
	liveViewPrepareTime(3.0f),
	previewShowingTime(1.4f),
	percent(0),
	framePosition(Vec2f(0.0f, 252.0f)),
	seekPosition(Vec2f::zero())
{
	cameraScale    = 1920.0f / 1056.0f;
	cameraWidth    = 1056 * cameraScale;
	cameraHeight   = 704 * cameraScale;
	cameraPosition = Vec2f((1080.0f - cameraHeight) * 0.5f, 0.0f);

	if (!photoTakenCon.connected())
		photoTakenCon = cameraCanon().photoTakenEvent.connect(bind(&PhotoShooting::photoTakenHandler, this));

	if (!photoDownloadedCon.connected())
		photoDownloadedCon = cameraCanon().photoDownloadedEvent.connect(bind(&PhotoShooting::photoDownloadHandler, this, std::placeholders::_1));

	if (!photoErrorCon.connected())
		photoErrorCon = cameraCanon().photoErrorEvent.connect(bind(&PhotoShooting::photoErrorHandler, this));
	
	maskShader = shadertool().getMaskShader();

	reset(settings);
};

PhotoShooting::~PhotoShooting()
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.Destruct ~~~");

	photoTakenCon.disconnect();
	photoDownloadedCon.disconnect();
	photoErrorCon.disconnect();
}

void PhotoShooting::start()
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.Start ~~~");

	cameraCanon().startLiveView();

	auto filterID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)filterID);

	photoStorage->clear();
	currentShot = 1;
	smileIndex = 0;

	progressBlockStartY = getWindowHeight() - 232.0f;
	progressBlockAnimateY = 300.0f;
	seekPosition = Vec2f::zero();
	percent = 0.0f;

	state = START_ANIM;
	maskSizeFinal = 1500.0f;
	maskSize = maskSizeInit = 100.0f;
	timeline().apply(&progressBlockAnimateY, 0.0f, 0.7f, EaseOutCubic()).delay(0.5f);
	timeline().apply(&maskSize, maskSizeInit, maskSizeFinal, 1.8f, EaseOutBack(1.70158f))
		.finishFn(bind(&PhotoShooting::showAnimationComplete, this));
}

void PhotoShooting::showAnimationComplete()
{
	callback(COMPLETE_ANIM);
	callDelayShotTimer();
}

void PhotoShooting::reset(PhotoboothSettingsRef settings)
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.Reset ~~~");

	IPhotoboothLocation::reset(settings);
	countsTex				  = settings->getTexture("counts");
	seekTex					  = settings->getTexture("seek");
	smileTexs				  = settings->getSmileTextures();
	line					  = settings->getTexture("shootline");
	frame					  = settings->getTexture("frame");
	shadow					  = settings->getTexture("shadow");
	backgroundProgresstexture = settings->getTexture("plash");	

	framePosition = Vec2f(0.5f * (getWindowWidth() - frame.getWidth()), 252.0f);
	countsTexPos  = Vec2f(0.5f * (getWindowWidth() - countsTex.getWidth()), 0.0f);
	seekTexPos0   = Vec2f(146.0f - seekTex.getWidth() * 0.5f, 0.5f * (countsTex.getHeight() - seekTex.getHeight()));
	photoTemplate = settings->getPhotoShootingCard();
}

void PhotoShooting::stop()
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.Stop ~~~");

	stopAllTweens();
	clearDelaycall();
}

void PhotoShooting::update()
{
	switch (state)
	{
	case PhotoShooting::PEPARE_FOR_SHOOTING:
		state = SHOOTING;
		cameraCanon().takePicture();
		break;

	case PhotoShooting::START_ANIM:
	case PhotoShooting::LIVE_VIEW:
		cameraCanon().update();
		break;
	}
}

void PhotoShooting::draw()
{
	fillBg();

	switch (state)
	{
	case PhotoShooting::START_ANIM:
		drawCameraTexture();
		drawDashedFrame();
		drawPhotoMaskIntro();	
		break;

	case PhotoShooting::LIVE_VIEW:
		drawCameraTexture();
		drawDashedFrame();
		break;

	case PhotoShooting::SHOOTING:
	case PhotoShooting::PEPARE_FOR_SHOOTING:
		gl::drawSolidRect(getWindowBounds());
		drawSmile();
		break;

	case PhotoShooting::PREVIEW:
		gl::drawSolidRect(getWindowBounds());

		if (previewAnimateX <= -0.2f)
			drawSmile();

		drawPhotoframe();
		break;
	}

	gl::color(Color::white());
	drawProgressBlock();
}

void PhotoShooting::drawCameraTexture()
{
	cameraTexture = cameraCanon().getTexture(cameraHeight, cameraWidth, 0, 0, cameraScale);
	gl::pushMatrices();
	gl::translate(cameraPosition);
	shader->render(cameraTexture);
	gl::popMatrices();
}

void PhotoShooting::drawDashedFrame()
{
	gl::draw(frame, framePosition);
}

void PhotoShooting::drawPhotoMaskIntro()
{
	ci::gl::Texture _photoMask;
	auto _photo = photoStorage->getLastScreenShot();

	if (_photo)
	{
		_photoMask = Utils::drawGraphicsToFBO(_photo.getSize(), [&]()
		{
			gl::drawSolidCircle(Vec2f(_photo.getWidth() * 0.5f, PhotoTimer::centerY), maskSize);
		});
		maskShader->render(_photo, _photoMask, Vec2f::zero(), 1);
	}		
}

void PhotoShooting::drawSmile()
{
	float smileY = 829.0f;
	gl::draw(smileTexs[smileIndex], Vec2f(0.5f * (getWindowWidth() - smileTexs[smileIndex].getWidth()), smileY - smileTexs[smileIndex].getHeight() * 0.5f));
}

void PhotoShooting::drawPhotoframe()
{
	auto startY = 294.0f;
	auto _scale = 748.0f / photoTemplate.getWidth();
	auto _scale1 = 748.0f / photo.getWidth();

	gl::pushMatrices();
	gl::translate(previewAnimateX, 0.0f);
	gl::pushMatrices();
	gl::pushMatrices();
	gl::translate(0.5f * (getWindowWidth() - shadow.getWidth()), startY - 150.0f);
	gl::draw(shadow);
	gl::popMatrices();

	gl::scale(_scale, _scale);
	gl::translate(0.5f * (getWindowWidth() * (1.0f / _scale) - photoTemplate.getWidth()), startY + 252.0f);
	gl::draw(photoTemplate);
	gl::popMatrices();

	gl::pushMatrices();
	gl::scale(_scale1, _scale1);
	gl::translate(0.5f * (getWindowWidth() * (1.0f / _scale1) - photo.getWidth()), startY);
	shader->render(photo);
	gl::popMatrices();
	gl::popMatrices();
}

void PhotoShooting::drawProgressBlock()
{
	gl::pushMatrices();
	gl::translate(0.0f, progressBlockStartY + progressBlockAnimateY);
	gl::draw(backgroundProgresstexture);	

	// draw progress line
	gl::color(Color::white());
	gl::draw(line, Vec2f(-line.getWidth() + getWindowWidth() * percent, 0.0f));	

	// draw counts
	gl::translate(0.0f, 95.0f);
	gl::draw(seekTex, seekTexPos0 + seekPosition);
	gl::draw(countsTex, countsTexPos);
	gl::popMatrices();
}

void PhotoShooting::photoTakenHandler()
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.PhotoTaken!!! ~~~");
}

void PhotoShooting::photoDownloadHandler(const string& path)
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.Downloaded ~~~");
	photo = photoStorage->loadDownloadedPhoto(path);
	callPreviewShowingTimer();
}

void PhotoShooting::photoErrorHandler()
{
	logger().log("~~~ Photobooth.SubLocation PhotoShooting.PhotoDownloadError!!!! ~~~");

	photo = cameraTexture;
	photoStorage->setNextPhoto(cameraTexture);
	delaycall(bind(&PhotoShooting::callPreviewShowingTimer, this), 1.0f);
}

void PhotoShooting::delayshoot()
{
	state = PEPARE_FOR_SHOOTING;
}

void PhotoShooting::previewdelay()
{
	cameraCanon().startLiveView();
	callLiveViewPrepareTimer();
}

void PhotoShooting::liveviewdelay()
{
	if (++currentShot <= SHOT_NUM)
		callDelayShotTimer();
	else
	{
		callback(BEGIN_ANIM);
		timeline().apply(&progressBlockAnimateY, 300.0f, 0.4f, EaseOutCubic())
			.finishFn(bind(&IPhotoboothLocation::hideAnimationComplete, this));
	}
}

void PhotoShooting::callDelayShotTimer()
{
	state = LIVE_VIEW;
	delaycall(bind(&PhotoShooting::delayshoot, this), delayShootingTime);
	timeline().apply(&percent, 0.0f, 1.0f, delayShootingTime);	
}

void PhotoShooting::callLiveViewPrepareTimer()
{
	delaycall(bind(&PhotoShooting::liveviewdelay, this), liveViewPrepareTime);
	timeline().apply(&previewAnimateX, 1500.0f, 1.0f, EaseInExpo()).delay(1.4f);

	if (currentShot < SHOT_NUM)
		timeline().apply(&seekPosition, Vec2f((currentShot) * 197.0f, 0.0f), 0.8f, EaseOutCubic()).delay(1.4f);
}

void PhotoShooting::callPreviewShowingTimer()
{
	state = PREVIEW;
	smileIndex = getNextSmileIndex();
	delaycall(bind(&PhotoShooting::previewdelay, this), previewShowingTime);
	previewAnimateX = -950.0f;
	timeline().apply(&previewAnimateX, 0.0f, 0.9f, EaseOutCubic()).delay(0.5f);
}

int PhotoShooting::getNextSmileIndex()
{
	if (++smileIndex > smileTexs.size() - 1)
		smileIndex = 0;

	return smileIndex;
}

void PhotoShooting::stopAllTweens()
{
	percent.stop();
	progressBlockAnimateY.stop();
	seekPosition.stop();
	maskSize.stop();
	IPhotoboothLocation::stopAllTweens();
}