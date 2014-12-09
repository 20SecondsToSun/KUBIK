#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class GameSettings:public ISettings
	{
	public:

		GameSettings(ApplicationModel *model)
		{
			console()<<"game settings create   "<<endl;
			this->model = model;
			currentGame = model->getDefaultGameID();
		}

		bool isGameCurrent(int id)
		{
			return currentGame == id;
		}

		int getCurrentGame()
		{
			return currentGame;
		}

		void setCurrentGame(int id)
		{
			currentGame =  id;
		}

		ISettings* getGameSettingsById()
		{
			ISettings *config;

			switch (currentGame)
			{
			case gameId::PHOTOBOOTH:		
				config = photoBoothSettings;			
				break;

			case gameId::FUNCES:
				config = funcesSettings;
				break;

			default:
				break;
			}

			return config;
		}

		FuncesSettings* getFuncesSettings()
		{
			return funcesSettings;
		}

		PhotoboothSettings* getPhotoboothSettings()
		{
			return photoBoothSettings;
		}

		Types::OneBlockTexDictionary getActiveGameTextures()
		{
			switch (currentGame)
			{
			case gameId::PHOTOBOOTH:
				return getPhotoboothTextures();

			case gameId::FUNCES:
				return getFuncesTextures();
			}			
		}

		void reload()
		{
			switch (currentGame)
			{
			case gameId::PHOTOBOOTH:
				photoBoothSettings->setTextures();
				break;
			case gameId::FUNCES:
				funcesSettings->setTextures();
				break;
			}	
		}

		void loadGamesSettings()
		{				
			vector<int> gameIDs = model->getGameIDsTurnOn();

			for (auto gameID: gameIDs)
			{
				switch (gameID)
				{
				case gameId::PHOTOBOOTH:
					parsePhotoboothSettings();
					break;

				case gameId::FUNCES:
					parseFuncesSettings();
					break;

				default:
					break;
				}				  
			}			
		}

		Types::OneBlockTexDictionary getPhotoboothTextures()
		{	
			return photoBoothSettings->getTextures();
		}

		Types::OneBlockTexDictionary getFuncesTextures()
		{	
			return funcesSettings->getTextures();
		}

		
		bool isGameID(int id)
		{
			return (id == gameId::PHOTOBOOTH || id == gameId::FUNCES);
		}

	private:

		int currentGame;
		ApplicationModel *model;
		FuncesSettings *funcesSettings;
		PhotoboothSettings *photoBoothSettings;

		void parsePhotoboothSettings()
		{
			console()<<"parse photobooth settings"<<endl;

			photoBoothSettings = new PhotoboothSettings();
			try	
			{				
				photoBoothSettings->load(model->getPhotoboothConfigPath());
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

		void parseFuncesSettings()
		{
			console()<<"parse funces settings"<<model->getFuncesConfigPath()<<endl;

			funcesSettings = new FuncesSettings();			
			try	
			{				
				funcesSettings->load(model->getFuncesConfigPath());
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

	};
}