#include "GameButton.h"
using namespace kubik::menu;

GameButton::GameButton(const GameData& data)
	:SimpleSpriteButton(data.getTexture().getWidth(), data.getTexture().getHeight(), data.getPosition()),
	texture(data.getTexture())
{
	console() << "getPosition() ::: " << data.getPosition()<<endl;
}

void GameButton::drawLayout()
{	
	gl::draw(texture);
}