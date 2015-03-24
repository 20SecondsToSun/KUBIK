#include "GameButton.h"
using namespace kubik::menu;

GameButton::GameButton(const GameData& data)
	:SimpleSpriteButton(data.getTexture().getWidth(), 
	data.getTexture().getHeight(),
	data.getPosition(),
	GameChoosedEventRef(new GameChoosedEvent(data.getID()))),
	texture(data.getTexture())
{	

}

void GameButton::drawLayout()
{	
	gl::draw(texture);
}