#include "PhotoboothSettings.h"

using namespace kubik::config;
using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

////////////////////////////////////////////////////////////////////////////
//
//				SET DATA
//
////////////////////////////////////////////////////////////////////////////

PhotoboothSettings::PhotoboothSettings(ApplicationModelRef model):ISettings(model)
{
		
}

void PhotoboothSettings::load()
{	
	logger().log("PhotoboothSettings settings load");
	mainConfigObj  = model->getConfigObject(settings::id::PHOTOBOOTH);

	loadConfigPaths();
	loadParams();
	loadLabels();
	loadConsts();
	loadDesignPath();

	logger().log("parse photoOverDesigns");
	parsePhotoOverDesigns();

	logger().log("parse photoCardStyles");
	parsePhotoCardStyles();

	logger().log("parse photoFilters");
	parsePhotoFiltersPreview();

	setTextures();		
}	

void PhotoboothSettings::loadConfigPaths()
{
	JsonTree pathJSON							= JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	configPaths.staticPartDesignPath			= pathJSON.getChild("staticPartDesignPath").getValue<string>();//"data\\interface\\"
	configPaths.kubikTemplatePartDesignPath		= pathJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();//"kubik\\templates\\"
	configPaths.userTemplatePartDesignPath		= pathJSON.getChild("userTemplatePartDesignPath").getValue<string>();//"user_design\\templates\\"
	configPaths.finalPath						= pathJSON.getChild("finalPath").getValue<string>();//"gameDesign\\photobooth\\"
	configPaths.stickersPath					= pathJSON.getChild("stickersPath").getValue<string>();
	configPaths.bgPrintsPath					= pathJSON.getChild("bgPrintsPath").getValue<string>();

	configPaths.photoOverDesignDataPath		    = pathJSON.getChild("photoOverDesignDataPath").getValue<string>();
	configPaths.photoCardsStylesDesignDataPath  = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
	configPaths.photoFiltersPreviewDesignDataPath = pathJSON.getChild("photoFiltersPreviewDesignDataPath").getValue<string>();	
}

void PhotoboothSettings::loadParams()
{
	JsonTree paramsJSON = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));	
	loadPhotoParams(paramsJSON);
	loadSocialParams(paramsJSON);
	loadPhotoFilterParams(paramsJSON);
	loadGameDesignParams(paramsJSON);
	loadGameStickerParams(paramsJSON);
	loadGameBgPrintParams(paramsJSON);
}

void PhotoboothSettings::loadLabels()
{
	JsonTree labelsJSON = JsonTree(loadFile(mainConfigObj.getLabelsConfigPath()));		
	loadConfigTexts(labelsJSON);
	loadSharingIcons(labelsJSON);
}

void PhotoboothSettings::loadConsts()
{
	JsonTree constsJSON = JsonTree(loadFile(mainConfigObj.getConstsConfigPath()));

	minPhotosShots = constsJSON.getChild("minPhotosShots").getValue<int>();
	maxPhotosShots = constsJSON.getChild("maxPhotosShots").getValue<int>();	
	minSecBetweenShots = constsJSON.getChild("minSecBetweenShots").getValue<int>();
	maxSecBetweenShots = constsJSON.getChild("maxSecBetweenShots").getValue<int>();			
	minCountTimer = constsJSON.getChild("minCountTimer").getValue<int>();
	maxCountTimer = constsJSON.getChild("maxCountTimer").getValue<int>();
}

void PhotoboothSettings::loadDesignPath()
{
	if(isCustomDesign)
		templateDesignPath = configPaths.userTemplatePartDesignPath; 
	else
		templateDesignPath = configPaths.kubikTemplatePartDesignPath;

	templateDesignPath += to_string(templateId) + "\\" + configPaths.finalPath;
	staticDesignPath	= configPaths.staticPartDesignPath  + configPaths.finalPath;
}

void PhotoboothSettings::loadPhotoParams(JsonTree config)
{
	seconds				  = config.getChild("seconds").getValue<int>();
	secondsBetweenShots	  = config.getChild("secondsBetweenShots").getValue<int>();
	photoNum			  = config.getChild("currentPhotoNum").getValue<int>();	
}

void PhotoboothSettings::loadSocialParams(JsonTree config)
{
	setSocialState(PhtTextID::FACEBOOK,	config.getChild("isFacebook").getValue<bool>());
	setSocialState(PhtTextID::VKONTAKTE,config.getChild("isVkotakte").getValue<bool>());
	setSocialState(PhtTextID::TWITTER,	config.getChild("isTwitter").getValue<bool>());
	setSocialState(PhtTextID::EMAIL,	config.getChild("isEmail").getValue<bool>());
	setSocialState(PhtTextID::QRCODE,	config.getChild("isQrCode").getValue<bool>());
	setSocialState(PhtTextID::PRINTER,	config.getChild("isPrint").getValue<bool>());
}

void PhotoboothSettings::loadPhotoFilterParams(JsonTree config)
{
	JsonTree datas  = JsonTree(config.getChild("filtersIds"));
	JsonTree useIds	= JsonTree(config.getChild("useIds"));

	vector<int> useIdsVec;

	for(auto it : useIds)
		useIdsVec.push_back(it.getChild("id").getValue<int>());

	for( auto it : datas)
	{
		Filter filter;
		filter.id	= it.getChild("id").getValue<int>();
		filter.isOn = findFilterId(filter.id, useIdsVec);
		filters.push_back(filter);

		activeFiltersIDs.push_back(filter.id);
	}
}		

void PhotoboothSettings::loadGameDesignParams(JsonTree config)
{
	templateId		= config.getChild("templateId").getValue<int>();		
	isCustomDesign	= config.getChild("isCustomDesign").getValue<bool>();	
}

void PhotoboothSettings::loadGameStickerParams(JsonTree config)
{
	activeOverDesignID = config.getChild("activeOverDesignID").getValue<int>();	

	isSticker		  = config.getChild("isSticker").getValue<bool>();			
	activeSticker.id  = config.getChild("activeSticker").getChild("id").getValue<int>();			
	findAllImagePrints(getBasePath().string() + configPaths.stickersPath,  stickers, true);
}

void PhotoboothSettings::loadGameBgPrintParams(JsonTree config)
{
	activePhotoCardStyleDesignID = config.getChild("activePhotoCardStyleDesignID").getValue<int>();	

	activeBgPrint.id = config.getChild("activeBgPrint").getChild("id").getValue<int>();
	findAllImagePrints(getBasePath().string() + configPaths.bgPrintsPath,  bgPrint, true);
}

void PhotoboothSettings::findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom) 
{
	for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();
			if(fileTools().isImageExtension(ext))
			{
				ImageElement imageElement;			
				imageElement.path	  = path + it->path().filename().string();
				imageElement.id		  = prints.size();
				prints.push_back(imageElement);
			}
		}
	}
}

void PhotoboothSettings::loadConfigTexts(JsonTree config)
{
	ConfigTexts<PhtTextID> txts;

	JsonTree jsonTexts = JsonTree(config.getChild("mainTitles"));
	for(auto it : jsonTexts)
	{
		string lang	    = it.getChild("lang").getValue<string>();										
		txts.insert(lang, PhtTextID::CARD_STYLE,  jtools().parseTextItem(it.getChild("photoCardDesign")));
		txts.insert(lang, PhtTextID::PRINT_COUNT, jtools().parseTextItem(it.getChild("photoPrintCount")));
		txts.insert(lang, PhtTextID::PHOTO_OVER,  jtools().parseTextItem(it.getChild("overElements")));
		txts.insert(lang, PhtTextID::FILTERS,     jtools().parseTextItem(it.getChild("photoFilters")));
		txts.insert(lang, PhtTextID::PUBLISHING,  jtools().parseTextItem(it.getChild("photoPublishing")));		
	}

	jsonTexts = JsonTree(config.getChild("subTitles"));
	for(auto it : jsonTexts)
	{
		string lang	    = it.getChild("lang").getValue<string>();										
		txts.insert(lang, PhtTextID::CARD_STYLE_SUB,  jtools().parseTextItem(it.getChild("photoCardDesign")));
		txts.insert(lang, PhtTextID::PRINT_COUNT_SUB, jtools().parseTextItem(it.getChild("photoPrintCount")));
		txts.insert(lang, PhtTextID::PHOTO_OVER_SUB,  jtools().parseTextItem(it.getChild("overElements")));
		txts.insert(lang, PhtTextID::FILTERS_SUB,     jtools().parseTextItem(it.getChild("photoFilters")));
		txts.insert(lang, PhtTextID::PUBLISHING_SUB,  jtools().parseTextItem(it.getChild("photoPublishing")));		
	}

	jsonTexts = JsonTree(config.getChild("social"));
	for(auto it : jsonTexts)
	{
		string lang	    = it.getChild("lang").getValue<string>();										
		txts.insert(lang, PhtTextID::EMAIL,     jtools().parseTextItem(it.getChild("email")));
		txts.insert(lang, PhtTextID::PRINTER,   jtools().parseTextItem(it.getChild("printer")));
		txts.insert(lang, PhtTextID::QRCODE,    jtools().parseTextItem(it.getChild("qr")));
		txts.insert(lang, PhtTextID::TWITTER,   jtools().parseTextItem(it.getChild("tw")));
		txts.insert(lang, PhtTextID::VKONTAKTE, jtools().parseTextItem(it.getChild("vk")));		
		txts.insert(lang, PhtTextID::FACEBOOK,  jtools().parseTextItem(it.getChild("fb")));		
	}

	jsonTexts = JsonTree(config.getChild("photos"));
	for(auto it : jsonTexts)
	{
		string lang	    = it.getChild("lang").getValue<string>();
		txts.insert(lang, PhtTextID::PHOTO_TREMPLATE_1,     jtools().parseTextItem(it.getChild("template1")));
		txts.insert(lang, PhtTextID::PHOTO_TREMPLATE_2,     jtools().parseTextItem(it.getChild("template2")));
	}

	configTexts = txts;
}

void PhotoboothSettings::loadSharingIcons(JsonTree config)
{
	typedef Pair<PhtTextID, string> NamePair;

	vector<NamePair> pairs;
	pairs.push_back(NamePair(PhtTextID::FACEBOOK,    "fbIcon" ));
	pairs.push_back(NamePair(PhtTextID::PRINTER,	 "printerIcon" ));	
	pairs.push_back(NamePair(PhtTextID::VKONTAKTE,   "vkIcon" ));	
	pairs.push_back(NamePair(PhtTextID::TWITTER,     "twIcon" ));	
	pairs.push_back(NamePair(PhtTextID::QRCODE,		 "qrIcon" ));	
	pairs.push_back(NamePair(PhtTextID::EMAIL,		 "emailIcon" ));	

	for (auto item : pairs)
	{
		string iconPath = config.getChild(item.param2).getValue<string>();
		sharing.setIcon(loadImage(getInterfacePath("configDesign\\photobooth\\" + iconPath)), item.param1);
	}

	string iconPath = config.getChild("ofIcon").getValue<string>();
	sharing.setEmptyIcon(loadImage(getInterfacePath("configDesign\\photobooth\\" + iconPath)));	
}

void PhotoboothSettings::parsePhotoOverDesigns()
{
	JsonTree designDataJSON	= JsonTree(loadFile(getBasePath().string() + configPaths.photoOverDesignDataPath));			
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
		photoOverDesignData.push_back(item);
	}
	userOverDesignID = designDataJSON.getChild("userDesignID").getValue<int>();	
}

void PhotoboothSettings::parsePhotoCardStyles()
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

void PhotoboothSettings::parsePhotoFiltersPreview()
{
	JsonTree designDataJSON	= JsonTree(loadFile(getBasePath().string() + configPaths.photoFiltersPreviewDesignDataPath));			
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
		photoFiltersPreview.push_back(item);
	}	
}

void PhotoboothSettings::buildData()
{
	logger().log("build photobooth");

	auto dic = configTexts.getDic();

	logger().log(to_string(dic.size()));		
	for (auto it = dic.begin(); it != dic.end(); ++it)	
		it->second.setFont(fonts);		

	configTexts.setDic(dic);
};

////////////////////////////////////////////////////////////////////////////
//
//			
//
////////////////////////////////////////////////////////////////////////////

Texture PhotoboothSettings::getActiveStickerTex()
{	
	//string name = STICKER_NAME + to_string(activeSticker.id);
	Texture tex;

	/*auto it = textures.find(name);

	if(it != textures.end())
		tex = textures[name]->get();*/

	return tex;
}

Texture PhotoboothSettings::getActivePrintBgTex()
{	
	//string name = PRINT_TEMPATE_NAME + to_string(activeBgPrint.id);	
	Texture tex;

	//auto it = textures.find(name);

	//if(it != textures.end())
	//	tex = textures[name]->get();

	return tex;
}

bool PhotoboothSettings::findFilterId(int id, vector<int> filters)
{
	for (auto it : filters)
		if(it == id)
			return true;

	return false;
}

int PhotoboothSettings::getPhotoCount(PhtTextID id)
{
	if(id == PhtTextID::PHOTO_TREMPLATE_1)
		return minPhotosShots;
	if(id == PhtTextID::PHOTO_TREMPLATE_2)
		return maxPhotosShots;

	return 0;
}

int PhotoboothSettings::getCurrentPhotoCount()
{
	return photoNum;
}

void PhotoboothSettings::setTextures()
{		
	clearResources();

	addToDictionary("closeImg",		    createImageResource(getStaticDesignPath("close.png")));
	addToDictionary("fon1",			    createImageResource(getTemplateDesignPath("PhotoInstruction\\1.jpg")));
	addToDictionary("fon2",			    createImageResource(getTemplateDesignPath("PhotoFilter\\1.jpg")));
	addToDictionary("fon3",			    createImageResource(getTemplateDesignPath("PhotoTimer\\1.jpg")));
	addToDictionary("helvetica40",      createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
	addToDictionary("helvetica100",     createFontResource(getFontsPath("Helvetica Neue.ttf"), 100));

	addToDictionary("introLight44",     createFontResource(getFontsPath("IntroLight.ttf"), 44));
	addToDictionary("helveticaLight24", createFontResource(getFontsPath("Helvetica Neue.ttf"), 24));
	addToDictionary("introLight30",     createFontResource(getFontsPath("IntroLight.ttf"), 30));
	addToDictionary("introBook30",      createFontResource(getFontsPath("Intro-Book.ttf"), 30));
	addToDictionary("introb210",		createFontResource(getFontsPath("introb.ttf"), 210));
	
	addToDictionary("photoTemplate1",	createImageResource(getInterfacePath("configDesign\\photobooth\\1photo.png")));
	addToDictionary("photoTemplate2",	createImageResource(getInterfacePath("configDesign\\photobooth\\3photo.png")));

	for (auto item : photoOverDesignData)	
	{
		std::string path = "configDesign\\photobooth\\" + item.getIconPath();
		console()<<"photoOverDesignData ::::::::::::::::::::::::"<<item.getName()<<"   "<<getInterfacePath(path)<<endl;
		addToDictionary(item.getName(),	createImageResource(getInterfacePath(path)));
	}

	for (auto item : photoCardStyles)	
	{
		std::string path = "configDesign\\photobooth\\" + item.getIconPath();
		console()<<" photoCardStyles ::::::::::::::::::::::::"<<item.getName()<<"   "<<getInterfacePath(path)<<endl;
		addToDictionary(item.getName(),	createImageResource(getInterfacePath(path)));
	}

	for (auto item : photoFiltersPreview)	
	{
		std::string path = "configDesign\\photobooth\\" + item.getIconPath();
		console()<<"photoFiltersPreview :::::::::::::::::::::::"<<item.getName()<<"   "<<getInterfacePath(path)<<endl;
		addToDictionary(item.getName(),	createImageResource(getInterfacePath(path)));
	}

	

	//for (size_t i = 0; i < stickers.size(); i++)
	//	addToDictionary(STICKER_NAME + to_string(i), createImageResource(stickers[i].path));

	//for (size_t i = 0; i < bgPrint.size(); i++)
	//	addToDictionary(PRINT_TEMPATE_NAME + to_string(i), createImageResource(bgPrint[i].path));	
}

std::vector<int> PhotoboothSettings::getOnFilters()
{
	std::vector<int> onFilters;

	for (auto filter: filters)			
		if(filter.isOn)				
			onFilters.push_back(filter.id);				

	return onFilters;
}

int PhotoboothSettings::getPhotoShots()
{
	return photoNum + 2;// only 1 or 3 photo makes
}

void PhotoboothSettings::saveConfig()
{
	fs::path basePath(mainConfigObj.getParamsConfigPath());
	JsonTree doc;
	Sharing sharing = sharing;

	doc.addChild(JsonTree("isFacebook", sharing.getSocialState(PhtTextID::FACEBOOK)));		
	doc.addChild(JsonTree("isVkotakte", sharing.getSocialState(PhtTextID::VKONTAKTE)));		
	doc.addChild(JsonTree("isTwitter",  sharing.getSocialState(PhtTextID::TWITTER)));		
	doc.addChild(JsonTree("isEmail",    sharing.getSocialState(PhtTextID::EMAIL)));		
	doc.addChild(JsonTree("isQrCode",	sharing.getSocialState(PhtTextID::QRCODE)));		
	doc.addChild(JsonTree("isPrint",	sharing.getSocialState(PhtTextID::PRINTER)));

	doc.addChild(JsonTree("photoNum",			  photoNum));		
	doc.addChild(JsonTree("seconds",			  seconds));		
	doc.addChild(JsonTree("secondsBetweenShots",  secondsBetweenShots));

	doc.addChild(JsonTree("templateId",			  templateId));
	doc.addChild(JsonTree("isCustomDesign",	      isCustomDesign));
	doc.addChild(JsonTree("isSticker",		      isSticker));

	JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");
	JsonTree useIdsJ	 = JsonTree::makeArray("useIds");

	for (auto it: filters)
	{
		JsonTree id;
		id.addChild(JsonTree("id", it.id));
		filtersIdsJ.pushBack( id);

		if(it.isOn)
		{
			JsonTree id1;
			id1.addChild(JsonTree("id", it.id));
			useIdsJ.pushBack( id1);
		}
	}	

	JsonTree sticker = JsonTree::makeObject("activeSticker");
	sticker.addChild(JsonTree("id", activeSticker.id));
	doc.addChild(sticker);		

	JsonTree bgPrint = JsonTree::makeObject("activeBgPrint");
	bgPrint.addChild(JsonTree("id", activeBgPrint.id));
	doc.addChild(bgPrint);	

	doc.addChild(filtersIdsJ);
	doc.addChild(useIdsJ);
	doc.write( writeFile(basePath), JsonTree::WriteOptions() );
}

TextItem PhotoboothSettings::getMainTitle(PhtTextID id)
{
	return configTexts.get(model->getLang(), id);//data.getTexts().get(model->getLang(), id);
}

TextItem PhotoboothSettings::getSubTitleClose(PhtTextID id)
{
	PhtTextID subID = static_cast<PhtTextID>(int(id) + 1);
	TextItem tItem = configTexts.get(model->getLang(), subID);

	if (subID == PHOTO_OVER_SUB)
		tItem.setText(getActiveOverDesignText());
	else if (subID == CARD_STYLE_SUB)
		tItem.setText(getActiveCardStyleText());
	else if (subID == FILTERS_SUB)	
		tItem.setText(getActiveFiltersTexts());
	else if (subID == PUBLISHING_SUB)	
		tItem.setText(getActivePublishingTexts());
	else if (subID == PRINT_COUNT_SUB)
	{
		if (photoNum == minPhotosShots)
			tItem.setText(to_string(photoNum) + " " + getTextItem(PhtTextID::PHOTO_TREMPLATE_1).getText());	
		else
			tItem.setText(to_string(photoNum) + " " + getTextItem(PhtTextID::PHOTO_TREMPLATE_1).getText());	
	}	

	return tItem;
}

std::string PhotoboothSettings::getActiveOverDesignText()
{
	for(auto item : photoOverDesignData)	
		if (item.getID() == activeOverDesignID)
			return item.getTextItem().getText();

	return "";	
}

std::string PhotoboothSettings::getActiveCardStyleText()
{
	for( auto item : photoCardStyles)	
		if (item.getID() == activePhotoCardStyleDesignID)
			return item.getTextItem().getText();

	return "";	
}

std::string PhotoboothSettings::getActiveFiltersTexts()
{
	std::string result = "";
	for( auto item : photoFiltersPreview)	
	{
		for (auto id : activeFiltersIDs)
		{
			if (item.getID() == id)
			{
				if (result.size())	result += ", ";
				result += item.getTextItem().getText();
			}
		}	
	}

	return result;	
}

std::string PhotoboothSettings::getActivePublishingTexts()
{
	std::string result = "";
	vector<PhtTextID> temp;
	temp.push_back(PhtTextID::PRINTER);
	temp.push_back(PhtTextID::EMAIL);
	temp.push_back(PhtTextID::QRCODE);
	temp.push_back(PhtTextID::FACEBOOK);
	temp.push_back(PhtTextID::VKONTAKTE);
	temp.push_back(PhtTextID::TWITTER);

	for (auto item : temp)
	{
		if (getSocialState(item))
		{	
			if (result.size())
				result += ", ";
			
			result += getTextItem(item).getText();		
		}
	}

	return result;
}

TextItem PhotoboothSettings::getSubTitleOpen(PhtTextID id)
{
	return configTexts.get(model->getLang(), static_cast<PhtTextID>(int(id) + 1));
}

TextItem PhotoboothSettings::getTextItem(PhtTextID id) 
{
	return configTexts.get(model->getLang(), id);
}		

bool PhotoboothSettings::getSocialState(PhtTextID id)
{
	return sharing.getSocialState(id);
}

void PhotoboothSettings::setSocialState(PhtTextID id, bool value)
{
	sharing.setSocialState(id, value);
}

ci::gl::Texture PhotoboothSettings::getIcon(PhtTextID id)
{
	return sharing.getIcon(id);
}

ci::gl::Texture PhotoboothSettings::getEmptyIcon() 
{
	return sharing.getEmptyIcon();
}

void PhotoboothSettings::Sharing::setSocialState(PhtTextID id, bool state)
{
	states[id] = state;				
}

bool PhotoboothSettings::Sharing::getSocialState(PhtTextID id)
{
	return states[id];
}

void PhotoboothSettings::Sharing::setIcon(ci::gl::Texture icon, PhtTextID id)
{
	icons[id] = icon;
}

ci::gl::Texture PhotoboothSettings::Sharing::getIcon(PhtTextID id)
{
	return icons[id];
}

void PhotoboothSettings::Sharing::setEmptyIcon(ci::gl::Texture icon)
{
	emptyIcon = icon;
}

ci::gl::Texture PhotoboothSettings::Sharing::getEmptyIcon()
{
	return emptyIcon;
}

int PhotoboothSettings::getActiveOverDesignID()
{
	return activeOverDesignID;
}

int PhotoboothSettings::getUserOverDesignID()
{
	return userOverDesignID;
}

int PhotoboothSettings::getActivePhotoCardStyleDesignID()
{
	return activePhotoCardStyleDesignID;
}

int PhotoboothSettings::getUserPhotoCardStyleDesignID()
{
	return userPhotoCardStyleDesignID;
}


