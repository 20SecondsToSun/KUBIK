#include "PhotoboothSettings.h"

using namespace kubik;

////////////////////////////////////////////////////////////////////////////
//
//				SET DATA
//
////////////////////////////////////////////////////////////////////////////

PhotoboothSettings::PhotoboothSettings(shared_ptr<ApplicationModel> model) 
{
	this->model = model;
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
	setConfigData(configJSON);
}

void PhotoboothSettings::setPhotoParams(JsonTree config)
{
	data.seconds				= config.getChild("seconds").getValue<int>();
	data.secondsBetweenShots	= config.getChild("secondsBetweenShots").getValue<int>();
	data.photoNum				= config.getChild("photoNum").getValue<int>();
}

void PhotoboothSettings::setSocialParams(JsonTree config)
{
	Sharing sharing;
	sharing.facebookOn				= config.getChild("isFacebook").getValue<bool>();
	sharing.vkotakteOn				= config.getChild("isVkotakte").getValue<bool>();
	sharing.twitterOn				= config.getChild("isTwitter").getValue<bool>();
	sharing.emailOn					= config.getChild("isEmail").getValue<bool>();
	sharing.qrCodeOn				= config.getChild("isQrCode").getValue<bool>();
	sharing.printOn					= config.getChild("isPrint").getValue<bool>();

	data.sharing = sharing;
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
	JsonTree mainTitles = JsonTree(config.getChild("mainTitles"));
	for(auto it : mainTitles)
	{
		string lang = it.getChild("lang").getValue<string>();
		ConfigTexts txts;
		txts.setDesignInterfaceText(it.getChild("text1").getValue<string>());
		txts.setPhotoStyleText(it.getChild("text2").getValue<string>());
		txts.setPhotoPrintCountText(it.getChild("text3").getValue<string>());
		txts.setPhotoOverElementsText(it.getChild("text4").getValue<string>());
		txts.setPhotoFiltersText(it.getChild("text5").getValue<string>());
		txts.setPublishText(it.getChild("text5").getValue<string>());

		data.setMainTitles(lang, txts);
	}

	JsonTree subTitles = JsonTree(config.getChild("subTitles"));
	for(auto it : subTitles)
	{
		string lang = it.getChild("lang").getValue<string>();
		ConfigTexts txts;
		txts.setDesignInterfaceText(it.getChild("text1").getValue<string>());
		txts.setPhotoStyleText(it.getChild("text2").getValue<string>());
		txts.setPhotoPrintCountText(it.getChild("text3").getValue<string>());
		txts.setPhotoOverElementsText(it.getChild("text4").getValue<string>());
		txts.setPhotoFiltersText(it.getChild("text5").getValue<string>());
		txts.setPublishText(it.getChild("text5").getValue<string>());

		data.setSubTitles(lang, txts);
	}

	JsonTree yourDesign = JsonTree(config.getChild("yourDesignText"));
	for(auto it : yourDesign)
	{
		string lang = it.getChild("lang").getValue<string>();
		StringVO yd;
			yd.set(it.getChild("text").getValue<string>());
		data.setYourDesignText(lang, yd);
	}

	JsonTree save = JsonTree(config.getChild("saveText"));
	for(auto it : save)
	{
		string lang = it.getChild("lang").getValue<string>();
		StringVO st;
			st.set(it.getChild("text").getValue<string>());
		data.setSaveText(lang, st);
	}


}

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

	addToDictionary("closeImg",		createImageResource(getStaticDesignPath("close.png")));
	addToDictionary("fon1",			createImageResource(getTemplateDesignPath("PhotoInstruction\\1.jpg")));
	addToDictionary("fon2",			createImageResource(getTemplateDesignPath("PhotoFilter\\1.jpg")));
	addToDictionary("fon3",			createImageResource(getTemplateDesignPath("PhotoTimer\\1.jpg")));
	addToDictionary("helvetica40",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
	addToDictionary("helvetica100", createFontResource(getFontsPath("Helvetica Neue.ttf"), 100));

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
	data = value;
	saveConfig();
}

void PhotoboothSettings::saveConfig()
{
	fs::path basePath(mainConfigPath + PARAMS_FILE);
	JsonTree doc;
	Sharing sharing = data.sharing;

	doc.addChild(JsonTree("isFacebook", sharing.facebookOn));		
	doc.addChild(JsonTree("isVkotakte", sharing.vkotakteOn));		
	doc.addChild(JsonTree("isTwitter",  sharing.twitterOn));		
	doc.addChild(JsonTree("isEmail",    sharing.emailOn));		
	doc.addChild(JsonTree("isQrCode",	sharing.qrCodeOn));		
	doc.addChild(JsonTree("isPrint",	sharing.printOn));

	doc.addChild(JsonTree("photoNum",			  data.photoNum));		
	doc.addChild(JsonTree("seconds",			  data.seconds));		
	doc.addChild(JsonTree("secondsBetweenShots",  data.secondsBetweenShots));

	doc.addChild(JsonTree("templateId",		  data.templateId));
	doc.addChild(JsonTree("isCustomDesign",	  data.isCustomDesign));
	doc.addChild(JsonTree("isSticker",		  data.isSticker));

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