#include "instakub/states/HashtagOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

HashtagOnly::HashtagOnly(InstakubSettingsRef settings)
	:InstakubLocation(settings),
	hashtagPlashka(settings->getTexture("hashtagPlashka")),
	hashtagPlashkaText(settings->getTexture("hashtagPlashkaText"))
{
	instagramView->setYPosition(332.0f);
	reset();
}

void HashtagOnly::reset()
{
	title = settings->getTexture("hashtagTitle");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight()*0.5f);	
}

void HashtagOnly::start()
{
	hashTagTexture = textTools().getTextField(settings->getHashtag(), &settings->getFont("introLight90"), Color::hex(0xaa9b74));
}

void HashtagOnly::draw()
{
	InstakubLocation::draw();
	gl::draw(title, titlePosition);
	instagramView->draw();

	float plashkaY = getWindowHeight() - hashtagPlashka.getHeight();

	gl::draw(hashtagPlashka, Vec2f(0.0f, plashkaY));

	gl::draw(hashtagPlashkaText, Vec2f(0.5f * (getWindowWidth() - hashtagPlashkaText.getWidth()), plashkaY + 90.0f - hashtagPlashkaText.getHeight()*0.5f));
	gl::draw(hashTagTexture, Vec2f(0.5f * (getWindowWidth() - hashTagTexture.getWidth()), plashkaY + 100.0f));
};