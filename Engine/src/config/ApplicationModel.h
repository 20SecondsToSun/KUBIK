#pragma once

#include "KubikException.h"
#include "Types.h"
#include "DesignData.h"
#include "ConfigObject.h"

namespace kubik
{
	namespace config
	{
		class ApplicationModel
		{

		public:
			void load();

			////////////////////////////////////////////////////////////////////////////
			//
			//					CONFIG PATH
			//
			////////////////////////////////////////////////////////////////////////////

			void parseConfigPaths();

			////////////////////////////////////////////////////////////////////////////
			//
			//					USER DATA
			//
			////////////////////////////////////////////////////////////////////////////

			void parseUserData();

			////////////////////////////////////////////////////////////////////////////
			//
			//					DESIGN DATA
			//
			////////////////////////////////////////////////////////////////////////////

			void parseDesignData();

			////////////////////////////////////////////////////////////////////////////
			//
			//					SAVE
			//
			////////////////////////////////////////////////////////////////////////////

			void saveUserData();

			////////////////////////////////////////////////////////////////////////////
			//
			//				ACCESSORS
			//
			////////////////////////////////////////////////////////////////////////////

			bool findGameId(int id, const std::vector<int>& gamesTurnOn);
			std::vector<GamesInfo> getGames();
			void setGames(const std::vector<GamesInfo>& games);

			void setDefaultGameID(GameId value);
			GameId getDefaultGameID() const;

			bool onlyOneGameOn();
			GameId onlyOneGameOnID();

			////////////////////////////////////////////////////////////////////////////
			//
			//				SET
			//
			////////////////////////////////////////////////////////////////////////////

			void setMenuConfigPath(const std::string& path);
			void setScreenSaverPath(const std::string& path);
			void writeGamesData(const std::vector<GamesInfo>& games);

			////////////////////////////////////////////////////////////////////////////
			//
			//				GET
			//
			////////////////////////////////////////////////////////////////////////////			

			const ConfigObject& getConfigObject(const settings::id& id);

			std::string getLabelsPath();
			std::string getMenuConfigPath();
			std::string getScreenSaverConfigPath();
			std::string getFuncesConfigPath();
			std::string getInstagramConfigPath();
			std::string getKotopozaConfigPath();
			std::string getUserDataPath();
			std::string getDesignDataPath();			
			std::string getFullPath(const std::string& path) const;
			std::string getLang() const;
			DesignData  getDesignData() const;
			int			getUserDesignID() const;

			std::string getTemplatePath();
			std::string getInterfacePath();

		private:
			int			standID;
			bool		netConnection;
			std::string userID;
			std::string lang;
			std::string menuConfigPath;
			std::string labelsPath;
			std::string designDataPath;
			std::string screenSaverConfigPath;
			std::string photoboothConfigPath;
			std::string funcesConfigPath;
			std::string instagramConfigPath;
			std::string kotopozaConfigPath;
			std::string userDataPath, _userDataPath;
			std::string gamesPurchasedPath;
			std::string gamesTurnOnPath;
			std::string userTemplateDesignPath;
			std::string templatesDesignPath;
			std::string intarfaceDesignPath;		

			std::vector<GamesInfo> games;
			GameId defaultGameID;
			DesignData designData;
			int userDesignID;

			std::map<settings::id, ConfigObject> configObjectMap;
			ConfigObject photoboothConfigObject, instakubConfigObject, mainConfigObject;
			fs::path getConfigPath();
			void parseConfigPaths(ConfigObject& configObject, const JsonTree& json);
		};

		typedef std::shared_ptr<ApplicationModel> ApplicationModelRef;
	}
}