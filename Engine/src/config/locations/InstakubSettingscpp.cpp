#include "InstakubSettings.h"

using namespace kubik;
using namespace kubik::config;

InstakubSettings::InstakubSettings(ApplicationModelRef model):ISettings(model), memento(false)
{

}

void InstakubSettings::load()
{
	mainConfigObj  = model->getConfigObject(settings::id::INSTAKUB);

	loadConfigPaths();
	loadParams();
	loadLabels();				

	parsePhotoCardStyles();			

	setTextures();	
}

void InstakubSettings::loadConfigPaths()
{
	JsonTree pathJSON							= JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	configPaths.photoCardsStylesDesignDataPath  = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
	configPaths.userPhotoCardStylePath			= getBasePath().string() + pathJSON.getChild("userPhotoCardStylePath").getValue<string>();
}

void InstakubSettings::loadParams()
{
	JsonTree paramsJSON			  = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));	
	search				          = paramsJSON.getChild("search").getValue<bool>();
	hashtag						  = paramsJSON.getChild("hashtag").getValue<string>();
	activePhotoCardStyleDesignID  = paramsJSON.getChild("activePhotoCardStyleDesignID").getValue<int>();	
}

void InstakubSettings::loadLabels()
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

void InstakubSettings::parsePhotoCardStyles()
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

void InstakubSettings::setTextures()
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

void InstakubSettings::buildData()
{
	auto dic = configTexts.getDic();				

	for (auto it = dic.begin(); it != dic.end(); ++it)	
		it->second.setFont(fonts);		

	configTexts.setDic(dic);	
};

TextItem InstakubSettings::getTextItem(const InstaTextID& id) 
{
	return configTexts.get(model->getLang(), id);
}

bool InstakubSettings::getSearchFlag() const
{
	return search;
}

void InstakubSettings::setSearchFlag(bool value)
{
	search = value;
}

std::string InstakubSettings::getHashtag() const
{
	return hashtag;
}

void InstakubSettings::setHashtag(const std::string& value)
{
	hashtag = value;
}

DesignData InstakubSettings::getPhotoCardStyles() const
{
	return photoCardStyles;
}

int InstakubSettings::getActivePhotoCardStyleDesignID() const
{
	return activePhotoCardStyleDesignID;
}

void InstakubSettings::setActivePhotoCardStyleDesignID(int id)
{
	activePhotoCardStyleDesignID = id;
}			

int InstakubSettings::getUserPhotoCardStyleDesignID() const
{
	return userPhotoCardStyleDesignID;
}

std::string InstakubSettings::getUserPhotoCardStylePath() const
{
	return configPaths.userPhotoCardStylePath;
}

void InstakubSettings::writeConfig()
{
	if (memento)
	{
		if (activePhotoCardStyleDesignID_save != activePhotoCardStyleDesignID ||
			hashtag != hashtag_save ||
			search  != search_save)
		{
			fs::path basePath(mainConfigObj.getParamsConfigPath());
			JsonTree doc;
			doc.addChild(JsonTree("search", search));		
			doc.addChild(JsonTree("hashtag", hashtag));		
			doc.addChild(JsonTree("activePhotoCardStyleDesignID", activePhotoCardStyleDesignID));	
			doc.write( writeFile(basePath), JsonTree::WriteOptions());

			console()<<"WRITE INSTAKUB"<<basePath<<endl;						
			console()<<"activePhotoCardStyleDesignID"<<activePhotoCardStyleDesignID<<endl;						
		}
		memento = false;
	}				
}

void InstakubSettings::createMemento()
{
	activePhotoCardStyleDesignID_save = activePhotoCardStyleDesignID;
	search_save = search;
	hashtag_save = hashtag;
	memento = true;
}