#include "PhotoShooting.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::signals;
using namespace ci::app;

PhotoShooting::PhotoShooting(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage)
	:photoStorage(photoStorage), 
	progressBlockStartY(100.0f), 
	delayShootingTime(3.0f),
	liveViewPrepareTime(2.0f),
	previewShowingTime(1.1f),
	percent(0),
	framePosition(Vec2f(0.0f, 252.0f)),
	seekPosition(Vec2f::zero())
{
	cameraScale	   = 1920.0f / 1056.0f;
	cameraWidth    = 1056 * cameraScale;
	cameraHeight   = 704  * cameraScale;
	cameraPosition = Vec2f((1080.0f - cameraHeight) * 0.5f, 0.0f);

	reset(settings);
	cameraCanon().photoTakenEvent.connect(std::bind(&PhotoShooting::photoTakenHandler, this));
	cameraCanon().photoDownloadedEvent.connect(std::bind(&PhotoShooting::photoDownloadHandler, this, std::placeholders::_1));
	cameraCanon().photoErrorEvent.connect(bind(&PhotoShooting::photoErrorHandler, this));
};

void PhotoShooting::start()
{
	using namespace shaders::imagefilters;
	console()<<"start PhotoShooting"<<endl;

	cameraCanon().startLiveView();
	auto fID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)fID);

	photoStorage->clear();
	currentShot = 1;

	callDelayShotTimer();	
}

void PhotoShooting::reset(PhotoboothSettingsRef _settings)
{
	IPhotoboothLocation::reset(settings);
	countsTex   = settings->getTexture("counts");
	seekTex		= settings->getTexture("seek");
	smileTex	= settings->getTexture("smile");
	line		= settings->getTexture("shootline");
	frame		= settings->getTexture("frame");

	framePosition	= Vec2f((getWindowWidth() -  frame.getWidth()) * 0.5f, 252.0f);
	countsTexPos	= Vec2f(0.5 * (getWindowWidth() - countsTex.getWidth()), 0.0f);
	seekTexPos0		= Vec2f(146.0f - seekTex.getWidth() * 0.5f, 0.5f * (countsTex.getHeight() - seekTex.getHeight()));	
	photoTemplate	= settings->getPhotoCardStylesActiveTemplate()[1];
}

void PhotoShooting::update() 
{
	cameraCanon().update();	

	if(state == PEPARE_FOR_SHOOTING)
	{
		state = SHOOTING;
		cameraCanon().takePicture();
	}
}

void PhotoShooting::draw()
{
	fillBg();

	float _scale, _scale1, startY = 10.0f;

	switch (state)
	{
	case PhotoShooting::LIVE_VIEW:
		cameraTexture = cameraCanon().getTexture(cameraHeight, cameraWidth, 0, 0, cameraScale);
		gl::pushMatrices();
		gl::translate(cameraPosition);
		shader->render(cameraTexture);
		gl::popMatrices();
		//gl::draw(cameraTexture, cameraPosition);	
		drawDashedFrame();
		break;

	case PhotoShooting::SHOOTING:
	case PhotoShooting::PEPARE_FOR_SHOOTING:
		gl::draw(smileTex, Vec2f(0.5f * (getWindowWidth() - smileTex.getWidth()), startY));
		break;

	case PhotoShooting::PREVIEW:
		_scale  = 748.0f / photoTemplate.getWidth();
		_scale1 = 748.0f / photo.getWidth();

		gl::color(ColorA(1, 1, 1, alphaPreview));
		gl::pushMatrices();	
		gl::scale(_scale, _scale);	
		gl::translate( 0.5 * (getWindowWidth()*(1/_scale) - photoTemplate.getWidth()), startY);	
		gl::draw(photoTemplate);		
		gl::popMatrices();

		gl::pushMatrices();	
		gl::scale(_scale1, _scale1);	
		gl::translate( 0.5 * (getWindowWidth()*(1/_scale1) - photo.getWidth()), startY);	
		shader->render(photo);
		//gl::draw(photo);		
		gl::popMatrices();
		gl::color(Color::white());
		break;
	}
		
	gl::color(Color::white());
	drawProgressBlock();	
}

void PhotoShooting::drawProgressBlock()
{
	gl::pushMatrices();
	gl::translate(0.0f, progressBlockStartY);

	{
		gl::color(ColorA::hexA(0xD60e0d0a));
		gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 232.0f));
	}

	{
		//gl::color(Color::hex(0xe4cf97));
		//gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth()*percent, 9.0f));	
		gl::color(Color::white());
		gl::draw(line, Vec2f(-line.getWidth()  + getWindowWidth()*percent, 0.0f));		
	}

	gl::translate(0.0f, 69.0f);
	seekTexPos = seekTexPos0 + seekPosition;
	gl::draw(seekTex, seekTexPos);
	gl::draw(countsTex, countsTexPos);
	gl::popMatrices();
}

void PhotoShooting::drawDashedFrame()
{
	gl::draw(frame, framePosition);
}

void PhotoShooting::photoTakenHandler()
{
	console()<<"photoTakenHandler "<<endl;
}

void PhotoShooting::photoDownloadHandler(const string& path)
{	
	photo = photoStorage->loadDownloadedPhoto(path);
	callPreviewShowingTimer();
}

void PhotoShooting::photoErrorHandler()
{
	console()<<"photoErrorHandler "<<endl;
	photo = cameraTexture;
	photoStorage->setNextPhoto(cameraTexture);	
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
	if(++currentShot <= SHOT_NUM)
		callDelayShotTimer();	
	else
		nextLocationSignal();
}

void PhotoShooting::callDelayShotTimer()
{
	state = LIVE_VIEW;
	timeline().apply( &_time, 0.0f, 100.0f, delayShootingTime).finishFn(bind( &PhotoShooting::delayshoot, this));
	timeline().apply( &percent, 0.0f, 1.0f, delayShootingTime);	
	timeline().apply( &seekPosition, Vec2f((currentShot - 1) * 197.0f, 0.0f), 0.3f, EaseOutCubic());
}

void PhotoShooting::callLiveViewPrepareTimer()
{
	timeline().apply( &_time, 10.0f, liveViewPrepareTime).finishFn(bind( &PhotoShooting::liveviewdelay, this));	
}

void PhotoShooting::callPreviewShowingTimer()
{
	state = PREVIEW;
	timeline().apply( &_time, 10.0f, previewShowingTime).finishFn(bind( &PhotoShooting::previewdelay, this));	
	timeline().apply(&alphaPreview, 0.0f, 1.0f, 1.1f);//, EaseOutCubic());
}