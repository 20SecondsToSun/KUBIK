#include "Funces.h"

using namespace kubik;
using namespace kubik::games::funces;

Funces::Funces(ISettingsRef config)
{	
	console()<<":: FUNCES CREATED::"<<endl;
	init(config);
	setType(ScreenId::FUNCES);
}

Funces::~Funces()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Funces destructor~~~~~~~~~~~~~~~~~~~"<<endl;	
}

void Funces::start()
{
	console() << "START FUNCES!!!" << endl;

	initShowAnimation();
	initKinect();
}

void Funces::initKinect()
{
	kinect().create();
	kinect().start();
	kinect().enableHandTracking();

	if (kinect().deviceExist())
	{		
		kinect().getDevice()->connectBodyEventHandler([&](const Kinect2::BodyFrame frame)
		{
			mBodyFrame = frame;
		});

		kinect().getDevice()->connectBodyIndexEventHandler([&](const Kinect2::BodyIndexFrame frame)
		{
			mChannelBodyIndex = frame.getChannel();
		});

		kinect().getDevice()->connectDepthEventHandler([&](const Kinect2::DepthFrame frame)
		{
			mChannelDepth = frame.getChannel();
		});

		kinect().getDevice()->connectColorEventHandler([&](const Kinect2::ColorFrame& frame)
		{
			mSurfaceColor = frame.getSurface();
		});
	}
}

void Funces::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);
}

void Funces::stop()
{
	console() << "STOP FUNCES!!!" << endl;

	kinect().stop();

	if (kinect().deviceExist())
	{
		kinect().getDevice()->disconnectBodyEventHandler();
		kinect().getDevice()->disconnectDepthEventHandler();
		kinect().getDevice()->disconnectColorEventHandler();
	}	
}

void Funces::init(ISettingsRef config)
{
	settings = static_pointer_cast<FuncesSettings>(config);
}

void Funces::reset() 
{
	
}

void Funces::closeMouseUpHandler(IButton& button )
{	
	closeLocationSignal();
}

void Funces::draw()
{
	auto drawColorStream = [&]() -> void
	{
		if (mSurfaceColor)
		{
			gl::TextureRef tex = gl::Texture::create(mSurfaceColor);
			gl::color(ColorAf(Colorf::white(), 1.f));
			gl::draw(mSurfaceColor);
		}
	};

	auto drawDepthStream = [&]() -> void
	{
		if (mChannelDepth)
		{
			gl::enable(GL_TEXTURE_2D);
			gl::TextureRef tex = gl::Texture::create(Kinect2::channel16To8(mChannelDepth));
			gl::draw(tex);
			gl::disable(GL_TEXTURE_2D);
		}
	};

	auto drawBodyIndexStream = [&]() -> void
	{
		if (mChannelBodyIndex)
		{
			gl::enable(GL_TEXTURE_2D);
			gl::color(ColorAf(Colorf::white(), 0.35f));
			gl::TextureRef bodyColorSurf = gl::Texture::create(Kinect2::colorizeBodyIndex(mChannelBodyIndex));
			gl::draw(bodyColorSurf);
			gl::disable(GL_TEXTURE_2D);
			gl::color(ColorAf(Colorf::white(), 1.f));
		}
	};

	switch (state)
	{
	case SHOW_ANIM:
		screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		gl::color(Color::black());
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
		gl::draw(settings->getTexture("title"), Vec2f(200.0f, 200.0f));

		gl::translate(300, 300);
		drawDepthStream();
		//currentLocation->draw();
		gl::popMatrices();
		break;

	case DRAW:
		//currentLocation->draw();
		break;
	}
}
