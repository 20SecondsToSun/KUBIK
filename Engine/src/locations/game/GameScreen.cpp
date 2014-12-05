#include "GameScreen.h"

using namespace kubik;

GameScreen::GameScreen(int gameID, ISettings* config)
{	
	this->gameID = gameID;

	switch (gameID)
	{
		case gameId::FUNCES:
			currentGame = shared_ptr<IGame>(new Funces(config));		
		break;

		case gameId::PHOTOBOOTH:
			currentGame = shared_ptr<IGame>(new Photobooth(config));
		break;

		//case 3:
			//currentGame = new Kotopoza();
		//break;

		default:
		break;
	}
	currentGame->closeGameSignal.connect(bind(&GameScreen::closeGameHandler, this));	
}

GameScreen::~GameScreen()
{
	console()<<"game screen destrpy------------------->"<<endl;
}

void GameScreen::closeGameHandler()
{
	closeGameSignal();
}

void GameScreen::draw()
{	
	currentGame->draw();
}

void GameScreen::init()
{
	currentGame->init();
}

void GameScreen::reset()
{
	currentGame->reset();
}

Types::OneBlockTexDictionary GameScreen::getTextures()
{
	return currentGame->getTextures();
}

void GameScreen::removeMouseUpListener()
{
	currentGame->removeMouseUpListener();
};

void GameScreen::addMouseUpListener()
{
	currentGame->addMouseUpListener();
};