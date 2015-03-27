#include "GameButton.h"

using namespace kubik::menu;
using namespace kubik::config;

GameButton::GameButton(const GameData& data)
	:SimpleSpriteButton(data.getTexture().getWidth(), 
	data.getTexture().getHeight(),
	data.getPosition(),
	GameChoosedEventRef(new GameChoosedEvent(data.getID()))),
	texture(data.getTexture())
{	
	console() << "create id:::::::::::::::::::  " << data.getID() << endl;
}

void GameButton::drawLayout()
{	
	gl::draw(texture);
}