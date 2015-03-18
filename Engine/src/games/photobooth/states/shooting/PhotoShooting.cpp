#include "PhotoShooting.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::signals;
using namespace ci::app;

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
	cameraScale = 1920.0f / 1056.0f;
	cameraWidth = 1056 * cameraScale;
	cameraHeight = 704 * cameraScale;
	cameraPosition = Vec2f((1080.0f - cameraHeight) * 0.5f, 0.0f);

	cameraCanon().photoTakenEvent.connect(std::bind(&PhotoShooting::photoTakenHandler, this));
	cameraCanon().photoDownloadedEvent.connect(std::bind(&PhotoShooting::photoDownloadHandler, this, std::placeholders::_1));
	cameraCanon().photoErrorEvent.connect(bind(&PhotoShooting::photoErrorHandler, this));

	using namespace shaders::imagefilters;
	maskShader = shadertool().getMaskShader();

	reset(settings);
};

void PhotoShooting::start()
{
	using namespace shaders::imagefilters;
	console() << "start PhotoShooting" << endl;

	cameraCanon().startLiveView();
	auto filterID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)filterID);

	photoStorage->clear();
	currentShot = 1;

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
	callDelayShotTimer();
}

void PhotoShooting::reset(PhotoboothSettingsRef settings)
{
	IPhotoboothLocation::reset(settings);
	countsTex = settings->getTexture("counts");
	seekTex = settings->getTexture("seek");
	smileTex = settings->getTexture("smile");
	line = settings->getTexture("shootline");
	frame = settings->getTexture("frame");

	framePosition = Vec2f((getWindowWidth() - frame.getWidth()) * 0.5f, 252.0f);
	countsTexPos = Vec2f(0.5f * (getWindowWidth() - countsTex.getWidth()), 0.0f);
	seekTexPos0 = Vec2f(146.0f - seekTex.getWidth() * 0.5f, 0.5f * (countsTex.getHeight() - seekTex.getHeight()));
	photoTemplate = settings->getPhotoCardStylesActiveTemplate()[1];
}

void PhotoShooting::stop()
{
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

	float _scale, _scale1, startY = 294.0f;// 10.0f;

	ci::gl::Texture _photo, _photoMask;

	switch (state)
	{
	case PhotoShooting::START_ANIM:
		drawCameraTexture();
		drawDashedFrame();

		_photo = photoStorage->getLastScreenShot();
		_photoMask = Utils::drawGraphicsToFBO(_photo.getSize(), [&]()
		{
			gl::drawSolidCircle(Vec2f(_photo.getWidth() * 0.5f, PhotoTimer::centerY), maskSize);
		});
		maskShader->render(_photo, _photoMask, Vec2f::zero(), 1);
		break;

	case PhotoShooting::LIVE_VIEW:
		drawCameraTexture();
		drawDashedFrame();
		break;

	case PhotoShooting::SHOOTING:
	case PhotoShooting::PEPARE_FOR_SHOOTING:
		gl::draw(smileTex, Vec2f(0.5f * (getWindowWidth() - smileTex.getWidth()), startY));
		break;

	case PhotoShooting::PREVIEW:
		_scale = 748.0f / photoTemplate.getWidth();
		_scale1 = 748.0f / photo.getWidth();
	
		gl::pushMatrices();
		gl::translate(previewAnimateX, 0.0f);
		gl::pushMatrices();
		gl::scale(_scale, _scale);
		gl::translate(0.5f * (getWindowWidth() * (1.0f / _scale) - photoTemplate.getWidth()), startY);
		gl::draw(photoTemplate);
		gl::popMatrices();

		gl::pushMatrices();
		gl::scale(_scale1, _scale1);
		gl::translate(0.5 * (getWindowWidth()*(1 / _scale1) - photo.getWidth()), startY);
		shader->render(photo);
		gl::popMatrices();
		gl::popMatrices();
		gl::color(Color::white());
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

void PhotoShooting::drawProgressBlock()
{
	gl::pushMatrices();
	gl::translate(0.0f, progressBlockStartY + progressBlockAnimateY);

	{//draw alpha bg
		gl::color(ColorA::hexA(0xD60e0d0a));
		gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 232.0f));
	}

	{// draw progress line
		gl::color(Color::white());
		gl::draw(line, Vec2f(-line.getWidth() + getWindowWidth() * percent, 0.0f));
	}

	// draw counts
	gl::translate(0.0f, 95.0f);
	gl::draw(seekTex, seekTexPos0 + seekPosition);
	gl::draw(countsTex, countsTexPos);
	gl::popMatrices();
}

void PhotoShooting::drawDashedFrame()
{
	gl::draw(frame, framePosition);
}

void PhotoShooting::photoTakenHandler()
{
	console() << "photoTakenHandler " << endl;
}

void PhotoShooting::photoDownloadHandler(const string& path)
{
	console() << "photoDownloadHandler " << endl;
	photo = photoStorage->loadDownloadedPhoto(path);
	callPreviewShowingTimer();
}

void PhotoShooting::photoErrorHandler()
{
	console() << "photoErrorHandler " << endl;
	photo = cameraTexture;
	photoStorage->setNextPhoto(cameraTexture);
	delaycall(bind(&PhotoShooting::callPreviewShowingTimer, this), 1.0f);
	//callPreviewShowingTimer();
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
		timeline().apply(&progressBlockAnimateY, 300.0f, 0.4f, EaseOutCubic())
			.finishFn(bind(&IPhotoboothLocation::hideAnimationComplete, this));
	}
}

void PhotoShooting::callDelayShotTimer()
{
	state = LIVE_VIEW;
	delaycall(bind(&PhotoShooting::delayshoot, this), delayShootingTime);

	timeline().apply(&percent, 0.0f, 1.0f, delayShootingTime);
	timeline().apply(&seekPosition, Vec2f((currentShot - 1) * 197.0f, 0.0f), 0.3f, EaseOutCubic());
}

void PhotoShooting::callLiveViewPrepareTimer()
{
	delaycall(bind(&PhotoShooting::liveviewdelay, this), liveViewPrepareTime);
	timeline().apply(&previewAnimateX, 1500.0f, 1.0f, EaseInExpo()).delay(1.4f);
}

void PhotoShooting::callPreviewShowingTimer()
{
	state = PREVIEW;

	delaycall(bind(&PhotoShooting::previewdelay, this), previewShowingTime);

	previewAnimateX = -950.0f;
	timeline().apply(&previewAnimateX, 0.0f, 0.9f, EaseOutCubic()).delay(0.5f);
}

void PhotoShooting::stopAllTweens()
{
	percent.stop();
	progressBlockAnimateY.stop();
	seekPosition.stop();
	maskSize.stop();
	IPhotoboothLocation::stopAllTweens();
}