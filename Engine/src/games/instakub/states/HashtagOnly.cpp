#include "instakub/states/HashtagOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

const float HashtagOnly::TIME_TO_BACK_PLASHKA = 10.0f;

HashtagOnly::HashtagOnly(InstakubSettingsRef settings):InstakubLocation(settings, Vec2f(18.0f, 332.0f))	
{
	reset();
}

void HashtagOnly::reset()
{
	InstakubLocation::reset();
	InstakubLocation::initOverMask();
	hashtagPlashka = settings->getTexture("hashtagPlashka");
	hashtagPlashkaText = settings->getTexture("hashtagPlashkaText");
	title = settings->getTexture("hashtagTitle");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
}

void HashtagOnly::start()
{
	TextItem hashtag = settings->getHashtag();

	hashTagTexture = textTools().getTextField(HASH_SYMBOL + hashtag.getText(), &hashtag.getFont(), hashtag.getColor());

	float plashkaY = getWindowHeight() - hashtagPlashka.getHeight();
	hashTagTexturePos = Vec2f(0.5f * (getWindowWidth() - hashTagTexture.getWidth()), plashkaY + 100.0f);
	hashtagPlashkaPos = Vec2f(0.0f, plashkaY);

	hashtagPlashkaTextPos = Vec2f(0.5f * (getWindowWidth() - hashtagPlashkaText.getWidth()), plashkaY + 90.0f - hashtagPlashkaText.getHeight()*0.5f);

	InstakubLocation::initPosition();
	InstakubLocation::start();

	instaViewer->setTopDragVisible(true);

	timeline().apply(&hashtagAnimPosition, Vec2f(0.0f, hashtagPlashka.getHeight()), Vec2f::zero(), 0.8f, EaseOutCubic());
}

void HashtagOnly::load()
{
	mode = HASHTAG_DEFAULT_PHOTOS_LOAD;
	loadStrategity();
}

void HashtagOnly::stop()
{
	InstakubLocation::stop();
	instaViewer->touchedDownEvent.disconnect_all_slots();
}

void HashtagOnly::draw()
{
	InstakubLocation::draw();
	gl::color(Color::white());
	gl::draw(overMask);
	gl::draw(title, titlePosition);		
	
	gl::pushMatrices();
	gl::translate(hashtagAnimPosition);
	gl::draw(hashtagPlashka, hashtagPlashkaPos);
	gl::draw(hashtagPlashkaText, hashtagPlashkaTextPos);
	gl::draw(hashTagTexture, hashTagTexturePos);
	gl::popMatrices();

	InstakubLocation::drawPopup();
};

void HashtagOnly::timeOutReload()
{
	InstakubLocation::reload();	
}

void HashtagOnly::initViewerHandlers()
{
	connect_once(instaViewer->touchedDownEvent, bind(&HashtagOnly::touchDownHandler, this));
	InstakubLocation::initViewerHandlers();
}

void HashtagOnly::touchDownHandler()
{
	timeline().apply(&hashtagAnimPosition, Vec2f(0.0f, hashtagPlashka.getHeight()), 0.8f, EaseOutCubic())
		.finishFn([&]()
	{
		timeline().apply(&hashtagAnimPosition, Vec2f::zero(), 0.8f, EaseOutCubic()).delay(TIME_TO_BACK_PLASHKA);
	});	
}