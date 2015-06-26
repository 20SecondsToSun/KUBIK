#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"
#include "DesignData.h"
#include "JsonTools.h"
#include "ConfigTexts.h"
#include "StringTools.h"
#include "services/fontStorage/FontStorage.h"

namespace kubik
{	
	namespace config
	{
		class ConfigSettings:public ISettings
		{
		public:
			static enum TextID
			{
				SWITCH_OFF,
				CONFIG,
				STAT,
				NOTINSTALL,
				DESIGNMAIN,
				DESIGNSUB,
				FINANDSTART,
				PLAYED_COUNT,
				PRINTED_COUNT, 
				SOCIAL_COUNT,
				LOADMAIN,
				LOADSUB,
				LOAD,
				BACK,

				CATRINGE_CHANGED,
				PHOTO_LEFT,
				PRINTER_ASK_TITLE,
				PRINTER_ASK_SUB_TITLE,
				PRINTER_CLOSE_TITLE,
				PRINTER_OK_TITLE,

				PARTY_ASK_TITLE,
				PARTY_DESQR,
				PARTY_TITLE,
				PARTY_BEGIN,
				PARTY_CLOSE
			};

			struct ConfighDataStruct
			{
				std::string actionName;
				int playedCount;
				int printedCount;
				int publishedCount;
				int currentPhotosPrinted;
				int maxPhotosToPrint;
				int activeDesign;
				int userDesignID;
				ci::Color activeDesignColor;	

				/////////////////////////////////			

				std::string lang;
				void setLang(const std::string& lang);

				/////////////////////////////////

				ConfigTexts<TextID> configTexts;
				ConfigTexts<TextID> getTexts() const;
				void setTexts(const ConfigTexts<TextID>& val);

				/////////////////////////////////

				DesignData designData;
				DesignData getDesignData() const;

			}data, mementoData;

			bool memento;
			ConfigObject mainConfigObj;
			std::string userDesignPath, templateDesignPath;

		public:
			ConfigSettings(ApplicationModelRef model);

			//////////////////////////////////////////////////////////////////

			void setCurrentPhotosPrinted(int value);
			int getCurrentPhotosPrinted();

			//////////////////////////////////////////////////////////////////

			void setPlayedCount(int value);
			int getPlayedCount();

			//////////////////////////////////////////////////////////////////

			void setPrintedCount(int value);
			int getPrintedCount();

			//////////////////////////////////////////////////////////////////

			void setPuplishedCount(int value);
			int getPublishedCount();

			//////////////////////////////////////////////////////////////////			

			int  getActiveDesignID() const;
			void  setActiveDesignID(int id);

			//////////////////////////////////////////////////////////////////

			ci::Color getActiveDesignColor() const;
			TextItem getTextItem(const TextID& id);

			//////////////////////////////////////////////////////////////////

			std::string getActionName() const;
			void setActionName(const std::string& value);
			DesignData getDesignData() const;
			std::string getUserDesignPath();
			ConfighDataStruct getData()	const;
			int getUserDesignID() const;
			void createMemento();
			bool gamesChanged() const;

			//////////////////////////////////////////////////////////////////

			void load() override;
			void loadPaths();
			std::string getInterfaceDesign() const;
			std::string getTemplateDesign();

			void loadConsts();
			void loadParams();
			void loadLabels();
			void setTextures();
			void buildData();
			void writeConfig();
			bool settingsChanged();
			bool designChanged();
			changeSetting::id getChangeID() const;
		};

		typedef ConfigSettings::TextID ConfigTextID;
		typedef std::shared_ptr<ConfigSettings> ConfigSettingsRef;	
	}
}