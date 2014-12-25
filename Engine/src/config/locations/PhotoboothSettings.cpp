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
	configPaths.kubikStickersPath				= configJSON.getChild("kubikStickersPath").getValue<string>();
	configPaths.userStickersPath				= configJSON.getChild("userStickersPath").getValue<string>();
	configPaths.userBgPrintsPath				= configJSON.getChild("userBgPrintsPath").getValue<string>();
	configPaths.kubikBgPrintsPath				= configJSON.getChild("kubikBgPrintsPath").getValue<string>();
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
}

void PhotoboothSettings::setPhotoParams(JsonTree config)
{
	data.seconds				= config.getChild("seconds").getValue<int>();
	data.secondsBetweenShots	= config.getChild("secondsBetweenShots").getValue<int>();
	data.photoNum				= config.getChild("photoNum").getValue<int>();
}

void PhotoboothSettings::setSocialParams(JsonTree config)
{
	SharingStruct sharing;
	sharing.isFacebook				= config.getChild("isFacebook").getValue<bool>();
	sharing.isVkotakte				= config.getChild("isVkotakte").getValue<bool>();
	sharing.isTwitter				= config.getChild("isTwitter").getValue<bool>();
	sharing.isEmail					= config.getChild("isEmail").getValue<bool>();
	sharing.isQrCode				= config.getChild("isQrCode").getValue<bool>();
	sharing.isPrint					= config.getChild("isPrint").getValue<bool>();

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
	data.templateId						= config.getChild("templateId").getValue<int>();		
	data.isCustomDesign					= config.getChild("isCustomDesign").getValue<bool>();	
}

void PhotoboothSettings::setGameStickerParams(JsonTree config)
{
	data.isSticker						= config.getChild("isSticker").getValue<bool>();			
	data.activeSticker.isCustom			= config.getChild("activeSticker").getChild("isCustom").getValue<bool>();
	data.activeSticker.id				= config.getChild("activeSticker").getChild("id").getValue<int>();			
	findAllImagePrints(getBasePath().string() + configPaths.userStickersPath,  data.customStickers, true);
	findAllImagePrints(getBasePath().string() + configPaths.kubikStickersPath, data.kubikStickers,  false);
}

void PhotoboothSettings::setGameBgPrintParams(JsonTree config)
{
	data.activeBgPrint.isCustom			= config.getChild("activeBgPrint").getChild("isCustom").getValue<bool>();
	data.activeBgPrint.id				= config.getChild("activeBgPrint").getChild("id").getValue<int>();
	findAllImagePrints(getBasePath().string() + configPaths.userBgPrintsPath,  data.customBgPrint, true);		
	findAllImagePrints(getBasePath().string() + configPaths.kubikBgPrintsPath, data.kubikBgPrint,  false);	
}

void PhotoboothSettings::findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom) 
{
	for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();
			if(ext == STICKER_SUPPORT_EXTENSION || ext == ".jpg")
			{
				ImageElement imageElement;
				imageElement.isCustom = isCustom;
				imageElement.path	  = path + it->path().filename().string();
				imageElement.id		  = prints.size();
				prints.push_back(imageElement);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//
//			
//
////////////////////////////////////////////////////////////////////////////

Texture PhotoboothSettings::getActiveStickerTex()
{
	Texture tex;
	string name;

	if(data.activeSticker.isCustom)
		name = CUSTOM_STICKER_NAME;
	else			
		name = KUBIK_STICKER_NAME;

	name += to_string(data.activeSticker.id);	

	auto it = textures.find(name);

	if(it != textures.end())
		tex = textures[name]->get();

	return tex;
}

Texture PhotoboothSettings::getActivePrintBgTex()
{
	Texture tex;
	string name;	

	if(data.activeBgPrint.isCustom)
		name = CUSTOM_PRINT_TEMPATE_NAME;
	else			
		name = KUBIK_PRINT_TEMPATE_NAME;

	name += to_string(data.activeBgPrint.id);	

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

	for (size_t i = 0; i < data.customStickers.size(); i++)
		addToDictionary(CUSTOM_STICKER_NAME + to_string(i), createImageResource(data.customStickers[i].path));	

	for (size_t i = 0; i < data.kubikStickers.size(); i++)	
		addToDictionary(KUBIK_STICKER_NAME + to_string(i), createImageResource(data.kubikStickers[i].path));

	for (size_t i = 0; i < data.customBgPrint.size(); i++)
		addToDictionary(CUSTOM_PRINT_TEMPATE_NAME + to_string(i), createImageResource(data.customBgPrint[i].path));	

	for (size_t i = 0; i < data.kubikBgPrint.size(); i++)	
		addToDictionary(KUBIK_PRINT_TEMPATE_NAME + to_string(i), createImageResource(data.kubikBgPrint[i].path));
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
	SharingStruct sharing = data.sharing;

	doc.addChild(JsonTree("isFacebook", sharing.isFacebook));		
	doc.addChild(JsonTree("isVkotakte", sharing.isVkotakte));		
	doc.addChild(JsonTree("isTwitter",  sharing.isTwitter));		
	doc.addChild(JsonTree("isEmail",    sharing.isEmail));		
	doc.addChild(JsonTree("isQrCode",	sharing.isQrCode));		
	doc.addChild(JsonTree("isPrint",	sharing.isPrint));

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
	sticker.addChild(JsonTree("isCustom", data.activeSticker.isCustom));
	sticker.addChild(JsonTree("id", data.activeSticker.id));
	doc.addChild(sticker);		

	JsonTree bgPrint = JsonTree::makeObject("activeBgPrint");
	bgPrint.addChild(JsonTree("isCustom", data.activeBgPrint.isCustom));
	bgPrint.addChild(JsonTree("id", data.activeBgPrint.id));
	doc.addChild(bgPrint);	

	doc.addChild(filtersIdsJ);
	doc.addChild(useIdsJ);
	doc.write( writeFile(basePath), JsonTree::WriteOptions() );
}