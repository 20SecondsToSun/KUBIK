#include "instakub/states/SearchButtons.h"

using namespace kubik;
using namespace ci;
using namespace kubik::games::instakub;
using namespace instagram;

SearchButtons::SearchButtons(const ci::gl::Texture& tex1,
	const ci::gl::Texture& tex2,
	const ci::gl::Texture& tex3,
	const ci::gl::Texture& tex4)
	:alpha(0.0f),
	hashtagTex(tex1),
	hashtagOverTex(tex2),
	userTex(tex3),
	userOverTex(tex4)
{
	hashTagButton = ImageButtonSpriteRef(new ImageButtonSprite(tex1));
	userButton = ImageButtonSpriteRef(new ImageButtonSprite(tex3));

	auto halfWindowWidthWidth = 1080.0f * 0.5f;
	hashTagButton->setPosition(Vec2i(halfWindowWidthWidth - tex1.getWidth(), 457.0f - tex1.getHeight()* 0.5f));
	userButton->setPosition(Vec2i(halfWindowWidthWidth, 457.0f - tex2.getHeight()*0.5f));

	userButton->setAlpha(alpha);
	hashTagButton->setAlpha(alpha);

	setDefaultSearchMode();
}

void SearchButtons::draw()
{
	userButton->setAlpha(alpha);
	hashTagButton->setAlpha(alpha);

	hashTagButton->draw();
	userButton->draw();
	gl::color(Color::white());
}

void SearchButtons::show()
{
	if (alpha == 0.0f)
	{
		timeline().apply(&alpha, 0.0f, 1.0f, 0.5f, EaseOutCubic());
	}		
}

void SearchButtons::hide()
{
	if (alpha == 1.0f)
	{
		timeline().apply(&alpha, 1.0f, 0.0f, 0.5f, EaseOutCubic());
	}
}

void SearchButtons::connect()
{
	hashTagButton->connectEventHandler(&SearchButtons::hashTagTouched, this);
	userButton->connectEventHandler(&SearchButtons::userTouched, this);
}

void SearchButtons::disconnect()
{
	hashTagButton->disconnectEventHandler();
	userButton->disconnectEventHandler();
}

void SearchButtons::hashTagTouched(EventGUIRef& event)
{
	setDefaultSearchMode();
}

void SearchButtons::userTouched(EventGUIRef& event)
{
	mode = USER;
	hashTagButton->changeTexture(hashtagTex);
	userButton->changeTexture(userOverTex);
}

void SearchButtons::setDefaultSearchMode()
{
	mode = HASHTAG;
	hashTagButton->changeTexture(hashtagOverTex);
	userButton->changeTexture(userTex);
}

SearchButtons::searchMode SearchButtons::getActiveSearchMode() const
{
	return mode;
}

bool SearchButtons::hashtagSearchMode() const
{
	return mode == HASHTAG;
}