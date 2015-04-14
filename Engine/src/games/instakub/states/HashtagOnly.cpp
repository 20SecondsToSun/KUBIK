#include "instakub/states/HashtagOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

HashtagOnly::HashtagOnly(InstakubSettingsRef settings)
	:InstakubLocation(settings, Vec2f(18.0f, 332.0f))	
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
	string hashtag = settings->getHashtag();
	hashTagTexture = textTools().getTextField(HASH_SYMBOL+ hashtag, &settings->getFont("introLight90"), Color::hex(0xaa9b74));

	float plashkaY = getWindowHeight() - hashtagPlashka.getHeight();
	hashTagTexturePos = Vec2f(0.5f * (getWindowWidth() - hashTagTexture.getWidth()), plashkaY + 100.0f);
	hashtagPlashkaPos = Vec2f(0.0f, plashkaY);

	hashtagPlashkaTextPos = Vec2f(0.5f * (getWindowWidth() - hashtagPlashkaText.getWidth()), plashkaY + 90.0f - hashtagPlashkaText.getHeight()*0.5f);

	InstakubLocation::initPosition();	
	InstakubLocation::start();
}

void HashtagOnly::load()
{
	mode = HASHTAG_DEFAULT_PHOTOS_LOAD;
	loadStrategity();
}

void HashtagOnly::stop()
{
	InstakubLocation::stop();
}

void HashtagOnly::draw()
{
	InstakubLocation::draw();
	gl::color(Color::white());
	gl::draw(overMask);
	gl::draw(title, titlePosition);	
	
	gl::draw(hashtagPlashka, hashtagPlashkaPos);
	gl::draw(hashtagPlashkaText, hashtagPlashkaTextPos);
	gl::draw(hashTagTexture, hashTagTexturePos);

	InstakubLocation::drawPopup();
};