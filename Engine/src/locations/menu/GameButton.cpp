#include "GameButton.h"

using namespace kubik::menu;
using namespace kubik::config;

GameButton::GameButton(const GameData& data, const ci::gl::Texture& texture, const ci::Vec2f& backgroundPosition,
	const ci::gl::Texture& title, const ci::Vec2f& titlePosition)
	:SimpleSpriteButton(data.getTexture().getWidth(), data.getTexture().getHeight(), data.getPosition(), GameChoosedEventRef(new GameChoosedEvent(data.getID()))),
	texture(data.getTexture()),
	background(texture),
	backgroundPosition(backgroundPosition),
	title(title),
	titlePosition(titlePosition)
{	
	console() << "create button game id:::::::::::::::::::  " << data.getID() << endl;
}

void GameButton::drawLayout()
{	
	gl::pushMatrices();
	gl::translate(backgroundPosition);
	gl::draw(background);
	gl::popMatrices();

	gl::draw(texture);

	gl::pushMatrices();
	gl::translate(titlePosition);
	gl::draw(title);
	gl::popMatrices();
}