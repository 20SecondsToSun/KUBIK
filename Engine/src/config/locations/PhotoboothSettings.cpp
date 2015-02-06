#include "PhotoboothSettings.h"

using namespace kubik::config;
using namespace kubik;

////////////////////////////////////////////////////////////////////////////
//
//				SET DATA
//
////////////////////////////////////////////////////////////////////////////

PhotoboothSettings::PhotoboothSettings(shared_ptr<ApplicationModel> model) 
{
	this->model     = model;
	mainConfigPath  = model->getPhotoboothConfigPath();
}

void PhotoboothSettings::load()
{	
	setConfigPaths();
	setParams();
	setDesignPath();
	setTextures();	
}	

void PhotoboothSettings::setConfigPaths()
{
	JsonTree configJSON = JsonTree(loadFile(mainConfigPath + CONFIG_FILE));
	configPaths.staticPartDesignPath			= configJSON.getChild("staticPartDesignPath").getValue<string>();//"data\\interface\\"
	configPaths.kubikTemplatePartDesignPath		= configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();//"kubik\\templates\\"
	configPaths.userTemplatePartDesignPath		= configJSON.getChild("userTemplatePartDesignPath").getValue<string>();//"user_design\\templates\\"
	configPaths.finalPath						= configJSON.getChild("finalPath").getValue<string>();//"gameDesign\\photobooth\\"
	configPaths.stickersPath					= configJSON.getChild("stickersPath").getValue<string>();
	configPaths.bgPrintsPath					= configJSON.getChild("bgPrintsPath").getValue<string>();
}

void PhotoboothSettings::setParams()
{
	JsonTree configJSON = JsonTree(loadFile(mainConfigPath + PARAMS_FILE));	
	setPhotoParams(configJSON);
	setSocialParams(configJSON);
	setPhotoFilterParams(configJSON);
	setGameDesignParams(configJSON);
	setGameStickerParams(configJSON);
	setGameBgPrintParams(configJSON);

	configJSON = JsonTree(loadFile(mainConfigPath + LABELS_FILE));		
	setConfigData(configJSON);
	setSharingIcons(configJSON);
}

void PhotoboothSettings::setPhotoParams(JsonTree config)
{
	data.seconds				  = config.getChild("seconds").getValue<int>();
	data.secondsBetweenShots	  = config.getChild("secondsBetweenShots").getValue<int>();
	data.photoNum				  = config.getChild("photoNum").getValue<int>();
}

void PhotoboothSettings::setSocialParams(JsonTree config)
{
	setSocialState(PhtTextID::FACEBOOK,	config.getChild("isFacebook").getValue<bool>());
	setSocialState(PhtTextID::VKONTAKTE, config.getChild("isVkotakte").getValue<bool>());
	setSocialState(PhtTextID::TWITTER,	config.getChild("isTwitter").getValue<bool>());
	setSocialState(PhtTextID::EMAIL,		config.getChild("isEmail").getValue<bool>());
	setSocialState(PhtTextID::QRCODE,	config.getChild("isQrCode").getValue<bool>());
	setSocialState(PhtTextID::PRINTER,	config.getChild("isPrint").getValue<bool>());
}

void PhotoboothSettings::setPhotoFilterParams(JsonTree config)
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
		data.filters.push_back(filter);
	}
}		

void PhotoboothSettings::setGameDesignParams(JsonTree config)
{
	data.templateId		= config.getChild("templateId").getValue<int>();		
	data.isCustomDesign	= config.getChild("isCustomDesign").getValue<bool>();	
}

void PhotoboothSettings::setGameStickerParams(JsonTree config)
{
	data.isSticker		  = config.getChild("isSticker").getValue<bool>();			
	data.activeSticker.id = config.getChild("activeSticker").getChild("id").getValue<int>();			
	findAllImagePrints(getBasePath().string() + configPaths.stickersPath,  data.stickers, true);
}

void PhotoboothSettings::setGameBgPrintParams(JsonTree config)
{
	data.activeBgPrint.id = config.getChild("activeBgPrint").getChild("id").getValue<int>();
	findAllImagePrints(getBasePath().string() + configPaths.bgPrintsPath,  data.bgPrint, true);
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

void PhotoboothSettings::setConfigData(JsonTree config)
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

	data.configTexts = txts;
}

void PhotoboothSettings::setSharingIcons(JsonTree config)
{
	Sharing sharing = data.sharing;
	typedef Pair<PhtTextID, string> NamePair;
	
	vector<NamePair> pairs;
	pairs.push_back(NamePair(PhtTextID::FACEBOOK,    "fbIcon" ));
	pairs.push_back(NamePair(PhtTextID::PRINTER, "printerIcon" ));	
	pairs.push_back(NamePair(PhtTextID::VKONTAKTE,    "vkIcon" ));	
	pairs.push_back(NamePair(PhtTextID::TWITTER,    "twIcon" ));	
	pairs.push_back(NamePair(PhtTextID::QRCODE,    "qrIcon" ));	
	pairs.push_back(NamePair(PhtTextID::EMAIL,   "emailIcon" ));	

	for (auto item : pairs)
	{
		string iconPath = config.getChild(item.param2).getValue<string>();
		sharing.setIcon(loadImage(getInterfacePath("configDesign\\photobooth\\" + iconPath)), item.param1);
	}
	
	string iconPath = config.getChild("ofIcon").getValue<string>();
	sharing.setEmptyIcon(loadImage(getInterfacePath("configDesign\\photobooth\\" + iconPath)));

	data.sharing = sharing;
}

void PhotoboothSettings::buildData()
{
	logger().log("build photobooth");

	auto texts = data.getTexts();
	auto dic   = texts.getDic();

	logger().log(to_string(dic.size()));		
	for (auto it = dic.begin(); it != dic.end(); ++it)	
		it->second.setFont(fonts);		
			
	texts.setDic(dic);		
	data.setTexts(texts);
};

////////////////////////////////////////////////////////////////////////////
//
//			
//
////////////////////////////////////////////////////////////////////////////

Texture PhotoboothSettings::getActiveStickerTex()
{	
	string name = STICKER_NAME + to_string(data.activeSticker.id);
	Texture tex;

	auto it = textures.find(name);

	if(it != textures.end())
		tex = textures[name]->get();

	return tex;
}

Texture PhotoboothSettings::getActivePrintBgTex()
{	
	string name = PRINT_TEMPATE_NAME + to_string(data.activeBgPrint.id);	
	Texture tex;

	auto it = textures.find(name);

	if(it != textures.end())
		tex = textures[name]->get();
	
	return tex;
}

bool PhotoboothSettings::findFilterId(int id, vector<int> filters)
{
	for (auto it : filters)
		if(it == id)
			return true;

	return false;
}	

void PhotoboothSettings::setDesignPath()
{
	if(data.isCustomDesign)
		templateDesignPath = configPaths.userTemplatePartDesignPath; 
	else
		templateDesignPath = configPaths.kubikTemplatePartDesignPath;

	templateDesignPath += to_string(data.templateId) + "\\" + configPaths.finalPath;
	staticDesignPath	= configPaths.staticPartDesignPath  + configPaths.finalPath;
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
	addToDictionary("_shareTemp",		createImageResource(getInterfacePath("configDesign\\photobooth\\_shareTemp.png")));



	for (size_t i = 0; i < data.stickers.size(); i++)
		addToDictionary(STICKER_NAME + to_string(i), createImageResource(data.stickers[i].path));
	
	for (size_t i = 0; i < data.bgPrint.size(); i++)
		addToDictionary(PRINT_TEMPATE_NAME + to_string(i), createImageResource(data.bgPrint[i].path));	
}

vector<int> PhotoboothSettings::getOnFilters()
{
	vector<int> onFilters;

	for (auto filter: data.filters)			
		if(filter.isOn)				
			onFilters.push_back(filter.id);				

	return onFilters;
}

int PhotoboothSettings::getPhotoShots()
{
	return data.photoNum + 2;// only 1 or 3 photo makes
}

PhotoboothSettings::PhotoboothDataStruct PhotoboothSettings::getData()
{
	return data;
}

void PhotoboothSettings::setData(PhotoboothDataStruct value)
{
	logger().log("set data");
	data = value;
	saveConfig();
}

void PhotoboothSettings::saveConfig()
{
	fs::path basePath(mainConfigPath + PARAMS_FILE);
	JsonTree doc;
	Sharing sharing = data.sharing;

	doc.addChild(JsonTree("isFacebook", sharing.getSocialState(PhtTextID::FACEBOOK)));		
	doc.addChild(JsonTree("isVkotakte", sharing.getSocialState(PhtTextID::VKONTAKTE)));		
	doc.addChild(JsonTree("isTwitter",  sharing.getSocialState(PhtTextID::TWITTER)));		
	doc.addChild(JsonTree("isEmail",    sharing.getSocialState(PhtTextID::EMAIL)));		
	doc.addChild(JsonTree("isQrCode",	sharing.getSocialState(PhtTextID::QRCODE)));		
	doc.addChild(JsonTree("isPrint",	sharing.getSocialState(PhtTextID::PRINTER)));

	doc.addChild(JsonTree("photoNum",			  data.photoNum));		
	doc.addChild(JsonTree("seconds",			  data.seconds));		
	doc.addChild(JsonTree("secondsBetweenShots",  data.secondsBetweenShots));

	doc.addChild(JsonTree("templateId",			  data.templateId));
	doc.addChild(JsonTree("isCustomDesign",	      data.isCustomDesign));
	doc.addChild(JsonTree("isSticker",		      data.isSticker));

	JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");
	JsonTree useIdsJ	 = JsonTree::makeArray("useIds");

	for (auto it: data.filters)
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
	sticker.addChild(JsonTree("id", data.activeSticker.id));
	doc.addChild(sticker);		

	JsonTree bgPrint = JsonTree::makeObject("activeBgPrint");
	bgPrint.addChild(JsonTree("id", data.activeBgPrint.id));
	doc.addChild(bgPrint);	

	doc.addChild(filtersIdsJ);
	doc.addChild(useIdsJ);
	doc.write( writeFile(basePath), JsonTree::WriteOptions() );
}