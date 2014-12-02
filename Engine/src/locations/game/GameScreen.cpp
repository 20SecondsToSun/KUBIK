#include "GameScreen.h"

GameScreen::GameScreen(int gameID)
{	
	this->gameID = gameID;

	switch (gameID)
	{
		case 1:
			currentGame = new Funces();		
		break;
		case 2:
			currentGame = new Photobooth();
		break;

		case 3:
			//currentGame = new Kotopoza();
		break;

		default:
		break;
	}
	currentGame->closeGameSignal.connect(bind(&GameScreen::closeGameHandler, this));	
}

GameScreen::~GameScreen()
{
	console()<<"game screen destrpy------------------->"<<endl;
	
	switch (gameID)
	{
		case 1:
			dynamic_cast<Funces*>(currentGame)->~Funces();
		break;
		case 2:
			dynamic_cast<Photobooth*>(currentGame)->~Photobooth();
		break;

		case 3:
			
		break;

		default:
		break;
	}	
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

OneBlockTexDictionary GameScreen::getTextures()
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