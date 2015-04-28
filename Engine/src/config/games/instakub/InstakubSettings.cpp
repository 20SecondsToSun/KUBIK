#include "InstakubSettings.h"

using namespace kubik;
using namespace kubik::config;

InstakubSettings::InstakubSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	configSettings(configSettings),
	memento(false)
{

}

void InstakubSettings::load()
{
	mainConfigObj = model->getConfigObject(settings::id::INSTAKUB);
	try
	{
		loadPaths();
		loadParams();
		loadLabels();
		loadConsts();	
		parsePhotoCardStyles();
	}
	catch (...)
	{
		throw ExcConfigFileParsing();
	}

	setTextures();
}

void InstakubSettings::loadPaths()
{
	JsonTree pathJSON = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	configPaths.photoCardsStylesDesignDataPath = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
	configPaths.userPhotoCardStylePath = getBasePath().string() + pathJSON.getChild("userPhotoCardStylePath").getValue<string>();
	configPaths.finalPath = pathJSON.getChild("finalPath").getValue<string>();
	staticDesignPath  = pathJSON.getChild("interfacePath").getValue<string>();	
}

void InstakubSettings::loadParams()
{
	JsonTree paramsJSON = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));
	search = paramsJSON.getChild("search").getValue<bool>();
	hashtag = paramsJSON.getChild("hashtag").getValue<string>();
	activePhotoCardStyleDesignID = paramsJSON.getChild("activePhotoCardStyleDesignID").getValue<int>();
}

void InstakubSettings::loadLabels()
{
	JsonTree labelsJSON = JsonTree(loadFile(mainConfigObj.getLabelsConfigPath()));

	JsonTree jsonTexts = JsonTree(labelsJSON.getChild("searchtitle"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, InstaTextID::SEARCH_TITLE_MAIN, jtools().parseTextItem(it.getChild("main")));
		configTexts.insert(lang, InstaTextID::SEARCH_TITLE_SUB, jtools().parseTextItem(it.getChild("sub")));
	}

	jsonTexts = JsonTree(labelsJSON.getChild("hashtagtitle"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, InstaTextID::HASHTAG_TITLE_MAIN, jtools().parseTextItem(it.getChild("main")));
		configTexts.insert(lang, InstaTextID::HASHTAG_TITLE_SUB, jtools().parseTextItem(it.getChild("sub")));
	}

	jsonTexts = JsonTree(labelsJSON.getChild("phototitle"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, InstaTextID::PHOTO_TITLE_MAIN, jtools().parseTextItem(it.getChild("main")));
		configTexts.insert(lang, InstaTextID::PHOTO_TITLE_SUB, jtools().parseTextItem(it.getChild("sub")));
	}
}

void InstakubSettings::loadConsts()
{
	JsonTree constsJSON = JsonTree(loadFile(mainConfigObj.getConstsConfigPath()));
	clientID = constsJSON.getChild("instagramClientID").getValue<string>();
}

void InstakubSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + configPaths.finalPath;
}

void InstakubSettings::parsePhotoCardStyles()
{
	JsonTree designDataJSON = JsonTree(loadFile(getBasePath().string() + configPaths.photoCardsStylesDesignDataPath));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());
		item.setDesignPath(it.getChild("designPath").getValue<string>());
		item.setIconTexName("instaElement" + item.getID());
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
	setDesignPath();
	clearResources();

	addToSettingsDictionary("introBook36", createFontResource(getFontsPath("Intro-Book.ttf"), 36));
	addToSettingsDictionary("introLight44", createFontResource(getFontsPath("IntroLight.ttf"), 44));
	addToSettingsDictionary("introLight36", createFontResource(getFontsPath("IntroLight.ttf"), 36));
	addToSettingsDictionary("introLight90", createFontResource(getFontsPath("IntroLight.ttf"), 90));
	addToSettingsDictionary("helveticaLight24", createFontResource(getFontsPath("HelveticaLight.ttf"), 24));
	addToSettingsDictionary("helveticaNeueLight24", createFontResource(getFontsPath("Helvetica Neue Light.ttf"), 24));

	addToSettingsDictionary("checkerw", createImageResource(getInterfacePath("configDesign\\instakub\\checkerw.png")));
	addToSettingsDictionary("searchfield", createImageResource(getInterfacePath("configDesign\\instakub\\searchfield.png")));
	addToSettingsDictionary("errorText", createImageResource(getInterfacePath("configDesign\\instakub\\errorText.png")));
	addToSettingsDictionary("closeKeyboardSettings", createImageResource(getInterfacePath("configDesign\\instakub\\closeKeyboard.png")));


	///////////////////////////////////////
	//
	///////////////////////////////////////


	addToDictionary("bg", createImageResource(getTemplateDesignPath("bg.png")));
	addToDictionary("hashtagTitle", createImageResource(getTemplateDesignPath("hashtagTitle.png")));
	addToDictionary("hashtagPlashka", createImageResource(getTemplateDesignPath("hashtagPlashka.png")));
	addToDictionary("hashtagPlashkaText", createImageResource(getTemplateDesignPath("hashtagPlashkaText.png")));
	addToDictionary("searchTitle", createImageResource(getTemplateDesignPath("searchTitle.png")));
	addToDictionary("searchField", createImageResource(getTemplateDesignPath("searchField.png")));
	addToDictionary("searchFieldError", createImageResource(getTemplateDesignPath("searchFieldError.png")));

	addToDictionary("hashtagSearch", createImageResource(getTemplateDesignPath("hashtagSearch.png")));
	addToDictionary("hashtagSearchOver", createImageResource(getTemplateDesignPath("hashtagSearchOver.png")));
	addToDictionary("userSearch", createImageResource(getTemplateDesignPath("userSearch.png")));
	addToDictionary("userSearchOver", createImageResource(getTemplateDesignPath("userSearchOver.png")));

	addToDictionary("closeInstaPopup", createImageResource(getTemplateDesignPath("closeInstaPopup.png")));
	addToDictionary("printInstaPopup", createImageResource(getTemplateDesignPath("printInstaPopup.png")));

	addToDictionary("notExistUser", createImageResource(getTemplateDesignPath("notExistUser.png")));
	addToDictionary("notPhotosUser", createImageResource(getTemplateDesignPath("notPhotosUser.png")));
	addToDictionary("noMaterials", createImageResource(getTemplateDesignPath("noMaterials.png")));
	addToDictionary("allLoaded", createImageResource(getTemplateDesignPath("allLoaded.png")));
	addToDictionary("privateUser", createImageResource(getTemplateDesignPath("privateUser.png")));
	addToDictionary("closeKeyboard", createImageResource(getTemplateDesignPath("closeKeyboard.png")));
	addToDictionary("pullupdate", createImageResource(getTemplateDesignPath("pullltoupdate.png")));
	addToDictionary("eraseInstagram", createImageResource(getTemplateDesignPath("eraseInstagram.png")));

	for (auto item : photoCardStyles)
	{
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));
		addToDictionary(item.getDesignTexName(), createImageResource(getBasePath().string() + item.getDesignPath()));
	}

	auto addPreloaderFilesToDictionary = [&](MovieLoader::MovieLoaderStruct& movieStruct, const string& name)
	{
		movieStruct = movieLoader().getMovieStruct(getTemplateDesignPath(name + "\\"), name);

		if (movieStruct.type == MovieLoader::VIDEO)		
			addToDictionary(movieStruct.name, createVideoResource(movieStruct.paths[0]));		
		else if (movieStruct.type == MovieLoader::IMAGE_SEQUENCE)		
			for (size_t i = 0; i < movieStruct.paths.size(); i++)
				addToDictionary(movieStruct.name + to_string(i), createImageResource(movieStruct.paths[i]));		
	};

	addPreloaderFilesToDictionary(mainPreloaderStruct, "mainpreloader");
	addPreloaderFilesToDictionary(miniPreloaderStruct, "minipreloader");
}

void InstakubSettings::buildLocationData()
{
	mainPreloader = movieLoader().getMovie(mainPreloaderStruct, getTextures(), getVideos());
	miniPreloader = movieLoader().getMovie(miniPreloaderStruct, getTextures(), getVideos());	

	for (auto &it : photoCardStyles)
	{
		auto tex = getTexture(it.getDesignTexName());
		it.setDesignTexture(tex);
	}
}

void InstakubSettings::buildSettingData()
{
	auto dic = configTexts.getDic();

	for (auto &it : dic)	
		it.second.setFont(fonts);	

	configTexts.setDic(dic);

	for (auto &it : photoCardStyles)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fonts);
	}
};

ci::gl::Texture InstakubSettings::getCurrentTemplate()
{
	auto iter = photoCardStyles.begin();
	std::advance(iter, activePhotoCardStyleDesignID - 1);

	return iter->getMappedTextures()[0];
}

IMovieRef InstakubSettings::getMainPreloader() const
{
	return mainPreloader;
}

IMovieRef InstakubSettings::getMiniPreloader() const
{
	return miniPreloader;
}

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

string InstakubSettings::getHashtag() const
{
	return hashtag;
}

void InstakubSettings::setHashtag(const string& value)
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

string InstakubSettings::getUserPhotoCardStylePath() const
{
	return configPaths.userPhotoCardStylePath;
}

string InstakubSettings::getClientID() const
{
	return clientID;
}

void InstakubSettings::writeConfig()
{
	if (memento)
	{		
		fs::path basePath(mainConfigObj.getParamsConfigPath());
		JsonTree doc;
		doc.addChild(JsonTree("search", search));
		doc.addChild(JsonTree("hashtag", hashtag));
		doc.addChild(JsonTree("activePhotoCardStyleDesignID", activePhotoCardStyleDesignID));
		doc.write(writeFile(basePath), JsonTree::WriteOptions());

		console() << "WRITE INSTAKUB" << basePath << endl;
		console() << "activePhotoCardStyleDesignID" << activePhotoCardStyleDesignID << endl;
	
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

bool InstakubSettings::settingsChanged()
{
	return activePhotoCardStyleDesignID_save != activePhotoCardStyleDesignID ||
		search_save != search ||
		hashtag_save != hashtag;
};

changeSetting::id InstakubSettings::getChangeID() const
{
	return changeSetting::id::INSTAKUB;
};

bool InstakubSettings::hashtagEnabled()
{
	return hashtag != "";
}

bool InstakubSettings::searchEnabled()
{
	return search;
}