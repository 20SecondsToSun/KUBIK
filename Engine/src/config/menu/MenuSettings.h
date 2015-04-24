#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class MenuSettings> MenuSettingsRef;

		class GameData
		{
			ci::gl::Texture texture;		
			ci::Vec2f position;
			GameId id;
			
		public:
			ci::gl::Texture getTexture() const;
			ci::Vec2f getPosition() const;
			GameId getID() const;

			friend class MenuSettings;
		};

		class MenuSettings :public ISettings
		{
		public:
			struct MenuDataStruct
			{
				bool isCustomDesign;
				int  templateId;

				std::string staticPartDesignPath;
				std::string kubikTemplatePartDesignPath;
				std::string userTemplatePartDesignPath;
				std::string finalPath;

				bool hasDesignChanges(const MenuDataStruct& menu);
			};

			MenuSettings(ApplicationModelRef model);
			virtual void createMemento(){};
			virtual void writeConfig(){};

			void load();
			void saveConfig();

			bool settingsChanged();
			changeSetting::id getChangeID() const;
			void setDesignPath();
			void setTextures();
			void setData(MenuDataStruct value);

			MenuDataStruct getData();

			std::vector<GamesInfo> getGames();
			std::vector<GameData> getEnabledGamesData();

		private:
			MenuDataStruct data;
			GameData gamesdata;
		};
	}
}