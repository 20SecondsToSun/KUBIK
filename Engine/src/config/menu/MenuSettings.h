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

		class AdditionalGameData
		{
			ci::gl::Texture background;
			ci::Vec2f backgroundPosition, titlePosition, iconPosition;

			std::map<GameId, ci::gl::Texture> titles;

		public:
			ci::gl::Texture getBackground() const;
			void setBackground(const ci::gl::Texture& tex);
			void addTitle(const GameId& value, const ci::gl::Texture& tex);

			ci::Vec2f getBackgroundPosition() const;
			ci::Vec2f getTitlePosition() const;
			ci::Vec2f getIconPosition() const;

			void setBackgroundPosition(const ci::Vec2f& value);
			void setTitlePosition(const ci::Vec2f& value);
			void setIconPosition(const ci::Vec2f& value);

			ci::gl::Texture getTitleByID(const GameId& value);

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
			void setDesignPath();
			void setTextures();
			void setData(MenuDataStruct value);

			bool settingsChanged();
			changeSetting::id getChangeID() const;
			MenuDataStruct getData();

			std::vector<GamesInfo> getGames();
			std::vector<GameData> getEnabledGamesData();
			AdditionalGameData getMenuScreenAdditionalDesignElements();

		private:
			MenuDataStruct data;
			GameData gamesdata;
		};
	}
}