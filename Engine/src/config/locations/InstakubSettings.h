#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"

namespace kubik
{
	namespace config
	{		
		typedef std::shared_ptr<class InstakubSettings> InstakubSettingsRef;
	
		class InstakubSettings: public ISettings
		{
		public:	

			enum InstaTextID
			{
				SEARCH_TITLE_MAIN, SEARCH_TITLE_SUB,
				HASHTAG_TITLE_MAIN,	HASHTAG_TITLE_SUB,
				PHOTO_TITLE_MAIN, PHOTO_TITLE_SUB				
			};

			InstakubSettings(ApplicationModelRef model):ISettings(model)
			{
		
			}

			virtual void load()
			{
				mainConfigObj  = model->getConfigObject(settings::id::INSTAKUB);

				loadConfigPaths();
				loadParams();
				loadLabels();				

				parsePhotoCardStyles();			

				setTextures();	
			}

			void loadConfigPaths()
			{
				JsonTree pathJSON							= JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
				configPaths.photoCardsStylesDesignDataPath  = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
			}

			void loadParams()
			{
				JsonTree paramsJSON			  = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));	
				search				          = paramsJSON.getChild("search").getValue<bool>();
				hashtag						  = paramsJSON.getChild("hashtag").getValue<string>();
				activePhotoCardStyleDesignID  = paramsJSON.getChild("activePhotoCardStyleDesignID").getValue<int>();	
			}

			void loadLabels()
			{
				JsonTree labelsJSON = JsonTree(loadFile(mainConfigObj.getLabelsConfigPath()));	

				JsonTree jsonTexts = JsonTree(labelsJSON.getChild("searchtitle"));
				for(auto it : jsonTexts)
				{
					string lang	    = it.getChild("lang").getValue<string>();
					configTexts.insert(lang, InstaTextID::SEARCH_TITLE_MAIN,     jtools().parseTextItem(it.getChild("main")));
					configTexts.insert(lang, InstaTextID::SEARCH_TITLE_SUB,      jtools().parseTextItem(it.getChild("sub")));
				}

				jsonTexts = JsonTree(labelsJSON.getChild("hashtagtitle"));
				for(auto it : jsonTexts)
				{
					string lang	    = it.getChild("lang").getValue<string>();
					configTexts.insert(lang, InstaTextID::HASHTAG_TITLE_MAIN,     jtools().parseTextItem(it.getChild("main")));
					configTexts.insert(lang, InstaTextID::HASHTAG_TITLE_SUB,      jtools().parseTextItem(it.getChild("sub")));
				}

				jsonTexts = JsonTree(labelsJSON.getChild("phototitle"));
				for(auto it : jsonTexts)
				{
					string lang	    = it.getChild("lang").getValue<string>();
					configTexts.insert(lang, InstaTextID::PHOTO_TITLE_MAIN,     jtools().parseTextItem(it.getChild("main")));
					configTexts.insert(lang, InstaTextID::PHOTO_TITLE_SUB,      jtools().parseTextItem(it.getChild("sub")));
				}
			}

			void parsePhotoCardStyles()
			{
				JsonTree designDataJSON	= JsonTree(loadFile(getBasePath().string() + configPaths.photoCardsStylesDesignDataPath));			
				JsonTree designs = designDataJSON.getChild("designs");			
		
				for(auto it : designs)
				{
					OneDesignItem item;
					item.setID(it.getChild("id").getValue<int>());
					item.setIconPath(it.getChild("iconPath").getValue<string>());
					JsonTree text = it.getChild("textObj");
			
					item.setTextItem(text.getChild("text").getValue<string>(),
											 text.getChild("font").getValue<string>(),
											 text.getChild("size").getValue<int>(),
											 text.getChild("color").getValue<string>());
					photoCardStyles.push_back(item);
				}
				userPhotoCardStyleDesignID = designDataJSON.getChild("userDesignID").getValue<int>();	
			}

			virtual void setTextures()
			{
				addToDictionary("introBook36",			createFontResource(getFontsPath("Intro-Book.ttf"), 36));
				addToDictionary("introLight44",			createFontResource(getFontsPath("IntroLight.ttf"), 44));
				addToDictionary("helveticaLight24",		createFontResource(getFontsPath("HelveticaLight.ttf"), 24));
				addToDictionary("helveticaNeueLight24", createFontResource(getFontsPath("Helvetica Neue Light.ttf"), 24));
				addToDictionary("checkerw",				createImageResource(getInterfacePath("configDesign\\instakub\\checkerw.png")));
				addToDictionary("searchfield",			createImageResource(getInterfacePath("configDesign\\instakub\\searchfield.png")));

				for (auto item : photoCardStyles)	
				{
					std::string path = item.getIconPath();
					console()<<" photoCardStyles ::::::::::::::::::::::::"<<item.getName()<<"   "<<getInterfacePath(path)<<endl;
					addToDictionary(item.getName(),	createImageResource(getInterfacePath(path)));
				}
			}

			virtual void buildData()
			{
				auto dic = configTexts.getDic();				
			
				for (auto it = dic.begin(); it != dic.end(); ++it)	
					it->second.setFont(fonts);		

				configTexts.setDic(dic);	
			};

			TextItem getTextItem(InstaTextID id) 
			{
				//console()<<"text::::::::::  "<<configTexts.get(model->getLang(), id).getText()<<endl;
				return configTexts.get(model->getLang(), id);
			}

			bool getSearchFlag()
			{
				return search;
			}

			DesignData getPhotoCardStyles()
			{
				return photoCardStyles;
			}

			int getActivePhotoCardStyleDesignID()
			{
				return activePhotoCardStyleDesignID;
			}

			int getUserPhotoCardStyleDesignID()
			{
				return userPhotoCardStyleDesignID;
			}

		private:
			std::string hashtag;
			bool search;
			int activePhotoCardStyleDesignID;
			int userPhotoCardStyleDesignID;

			ConfigTexts<InstaTextID> configTexts;

			ConfigObject mainConfigObj;
			DesignData photoCardStyles;

			class ConfigPath
			{			
				std::string photoCardsStylesDesignDataPath;

			public:
				friend InstakubSettings;
			}configPaths;
		};
	}
}