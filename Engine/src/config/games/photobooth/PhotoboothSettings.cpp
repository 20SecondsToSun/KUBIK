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

PhotoboothSettings::PhotoboothSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	memento(false),
	configSettings(configSettings)
{

}

void PhotoboothSettings::load()
{
	mainConfigObj = model->getConfigObject(settings::id::PHOTOBOOTH);

	console() << "start photobooth loader" << endl;
	try
	{
		loadPaths();
		loadParams();
		loadLabels();
		loadConsts();

		parsePhotoOverDesigns();
		parsePhotoCardStyles();
		parsePhotoFiltersPreview();
	}
	catch (...)
	{
		throw ExcConfigFileParsing();
	}

	setTextures();
}

void PhotoboothSettings::loadPaths()
{
	JsonTree pathJSON = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));

	configPaths.userStickerPath = pathJSON.getChild("userStickerPath").getValue<string>();
	configPaths.userCardStylePath = pathJSON.getChild("userCardStylePath").getValue<string>();
	configPaths.photoOverDesignDataPath = pathJSON.getChild("photoOverDesignDataPath").getValue<string>();
	configPaths.photoCardsStylesDesignDataPath = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
	configPaths.photoFiltersPreviewDesignDataPath = pathJSON.getChild("photoFiltersPreviewDesignDataPath").getValue<string>();
	configPaths.finalPath = pathJSON.getChild("finalPath").getValue<string>();

	staticDesignPath = pathJSON.getChild("interfacePath").getValue<string>();
}

void PhotoboothSettings::loadParams()
{
	JsonTree paramsJSON = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));
	loadSocialParams(paramsJSON);
	loadPhotoFilterParams(paramsJSON);
	loadGameDesignParams(paramsJSON);
}

void PhotoboothSettings::loadLabels()
{
	JsonTree labelsJSON = JsonTree(loadFile(mainConfigObj.getLabelsConfigPath()));
	loadConfigTexts(labelsJSON);
}

void PhotoboothSettings::loadConsts()
{
	JsonTree constsJSON = JsonTree(loadFile(mainConfigObj.getConstsConfigPath()));

	JsonTree rects = JsonTree(constsJSON.getChild("cardStylesRects"));
	for (auto it : rects)
	{
		RectT<int> rect;
		rect.x1 = it.getChild("x1").getValue<int>();
		rect.y1 = it.getChild("y1").getValue<int>();
		rect.x2 = it.getChild("x2").getValue<int>();
		rect.y2 = it.getChild("y2").getValue<int>();
		photoCardStylesCoordRects.push_back(rect);
	}

	JsonTree rects1 = JsonTree(constsJSON.getChild("photoOverRects"));
	for (auto it : rects1)
	{
		RectT<int> rect;
		rect.x1 = it.getChild("x1").getValue<int>();
		rect.y1 = it.getChild("y1").getValue<int>();
		rect.x2 = it.getChild("x2").getValue<int>();
		rect.y2 = it.getChild("y2").getValue<int>();
		photoOverCoordRects.push_back(rect);
	}

	loadSharingIcons(constsJSON);
}

void PhotoboothSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + configPaths.finalPath;
}

void PhotoboothSettings::loadSocialParams(JsonTree config)
{
	setSocialState(PhtTextID::FACEBOOK, config.getChild("isFacebook").getValue<bool>());
	setSocialState(PhtTextID::VKONTAKTE, config.getChild("isVkotakte").getValue<bool>());
	setSocialState(PhtTextID::TWITTER, config.getChild("isTwitter").getValue<bool>());
	setSocialState(PhtTextID::EMAIL, config.getChild("isEmail").getValue<bool>());
	setSocialState(PhtTextID::QRCODE, config.getChild("isQrCode").getValue<bool>());
	setSocialState(PhtTextID::PRINTER, config.getChild("isPrint").getValue<bool>());
}

void PhotoboothSettings::loadPhotoFilterParams(JsonTree config)
{
	JsonTree datas = JsonTree(config.getChild("filtersIds"));
	for (auto it : datas)
	{
		Filter filter;
		filter.id = it.getChild("id").getValue<int>();
		filter.isOn = it.getChild("isOn").getValue<bool>();
		filters.push_back(filter);
	}
}

void PhotoboothSettings::loadGameDesignParams(JsonTree config)
{
	activeOverDesignID = config.getChild("activeOverDesignID").getValue<int>();
	isSticker = config.getChild("isSticker").getValue<bool>();
	activeSticker.id = config.getChild("activeSticker").getValue<int>();
	activePhotoCardStyleDesignID = config.getChild("activePhotoCardStyleDesignID").getValue<int>();
	activeBgPrint.id = config.getChild("activeBgPrint").getValue<int>();
}

void PhotoboothSettings::loadConfigTexts(JsonTree config)
{
	JsonTree jsonTexts = JsonTree(config.getChild("mainTitles"));

	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::CARD_STYLE, jtools().parseTextItem(it.getChild("photoCardDesign")));
		configTexts.insert(lang, PhtTextID::PHOTO_OVER, jtools().parseTextItem(it.getChild("overElements")));
		configTexts.insert(lang, PhtTextID::FILTERS, jtools().parseTextItem(it.getChild("photoFilters")));
		configTexts.insert(lang, PhtTextID::PUBLISHING, jtools().parseTextItem(it.getChild("photoPublishing")));
	}

	jsonTexts = JsonTree(config.getChild("subTitles"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::CARD_STYLE_SUB, jtools().parseTextItem(it.getChild("photoCardDesign")));
		configTexts.insert(lang, PhtTextID::PHOTO_OVER_SUB, jtools().parseTextItem(it.getChild("overElements")));
		configTexts.insert(lang, PhtTextID::FILTERS_SUB, jtools().parseTextItem(it.getChild("photoFilters")));
		configTexts.insert(lang, PhtTextID::PUBLISHING_SUB, jtools().parseTextItem(it.getChild("photoPublishing")));
	}

	jsonTexts = JsonTree(config.getChild("social"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::EMAIL, jtools().parseTextItem(it.getChild("email")));
		configTexts.insert(lang, PhtTextID::PRINTER, jtools().parseTextItem(it.getChild("printer")));
		configTexts.insert(lang, PhtTextID::QRCODE, jtools().parseTextItem(it.getChild("qr")));
		configTexts.insert(lang, PhtTextID::TWITTER, jtools().parseTextItem(it.getChild("tw")));
		configTexts.insert(lang, PhtTextID::VKONTAKTE, jtools().parseTextItem(it.getChild("vk")));
		configTexts.insert(lang, PhtTextID::FACEBOOK, jtools().parseTextItem(it.getChild("fb")));
	}

	jsonTexts = JsonTree(config.getChild("filters_loc"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::FILTER_TEXT1, jtools().parseTextItem(it.getChild("title1")));
		configTexts.insert(lang, PhtTextID::FILTER_TEXT2, jtools().parseTextItem(it.getChild("title2")));
	}
}

void PhotoboothSettings::loadSharingIcons(JsonTree config)
{
	typedef Pair<PhtTextID, string> NamePair;

	vector<NamePair> pairs;
	pairs.push_back(NamePair(PhtTextID::FACEBOOK, "fbIcon"));
	pairs.push_back(NamePair(PhtTextID::PRINTER, "printerIcon"));
	pairs.push_back(NamePair(PhtTextID::VKONTAKTE, "vkIcon"));
	pairs.push_back(NamePair(PhtTextID::TWITTER, "twIcon"));
	pairs.push_back(NamePair(PhtTextID::QRCODE, "qrIcon"));
	pairs.push_back(NamePair(PhtTextID::EMAIL, "emailIcon"));

	for (auto item : pairs)
	{
		string iconPath = config.getChild(item.param2).getValue<string>();
		sharing.setIcon(loadImage(getInterfacePath("" + iconPath)), item.param1);
	}

	string iconPath = config.getChild("ofIcon").getValue<string>();
	sharing.setEmptyIcon(loadImage(getInterfacePath(iconPath)));
}

void PhotoboothSettings::parsePhotoOverDesigns()
{
	JsonTree designDataJSON = JsonTree(loadFile(getBasePath().string() + configPaths.photoOverDesignDataPath));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());
		item.setDesignPath(it.getChild("designPath").getValue<string>());
		item.setIconTexName("overElement" + item.getID());
		item.setDesignTexName("overDesignElement" + item.getID());

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
	JsonTree designDataJSON = JsonTree(loadFile(getBasePath().string() + configPaths.photoCardsStylesDesignDataPath));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());
		item.setDesignPath(it.getChild("designPath").getValue<string>());
		item.setIconTexName("cardStyle" + item.getID());
		item.setDesignTexName("cardDesignStyle" + item.getID());

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
	JsonTree designDataJSON = JsonTree(loadFile(getBasePath().string() + configPaths.photoFiltersPreviewDesignDataPath));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());
		item.setIconTexName("filter" + item.getID());

		JsonTree text = it.getChild("textObj");
		item.setTextItem(text.getChild("text").getValue<string>(),
			text.getChild("font").getValue<string>(),
			text.getChild("size").getValue<int>(),
			text.getChild("color").getValue<string>());
		photoFiltersPreview.push_back(item);
	}
}

////////////////////////////////////////////////////////////////////////////
//
//			
//
////////////////////////////////////////////////////////////////////////////

vector<Texture> PhotoboothSettings::getPhotoCardStylesActiveTemplate()
{
	auto iter = photoCardStyles.begin();
	std::advance(iter, activePhotoCardStyleDesignID - 1);

	return iter->getMappedTextures();
}

vector<Texture> PhotoboothSettings::getPhotoOverActiveTemplate()
{
	auto iter = photoOverDesignData.begin();
	std::advance(iter, activeOverDesignID - 1);
	return iter->getMappedTextures();
}

void PhotoboothSettings::setTextures()
{
	setDesignPath();
	clearResources();

	addToDictionary("instrFon", createImageResource(getTemplateDesignPath("PhotoInstruction\\screensaver\\1.jpg")));
	addToDictionary("instrTitle", createImageResource(getTemplateDesignPath("PhotoInstruction\\title\\title.png")));

	addToDictionary("filterTitle", createImageResource(getTemplateDesignPath("PhotoFilter\\title.png")));

	addToDictionary("timer1", createImageResource(getTemplateDesignPath("PhotoTimer\\timer1.png")));
	addToDictionary("timer2", createImageResource(getTemplateDesignPath("PhotoTimer\\timer2.png")));
	addToDictionary("timertitle", createImageResource(getTemplateDesignPath("PhotoTimer\\title.png")));
	addToDictionary("digit1", createImageResource(getTemplateDesignPath("PhotoTimer\\digit1.png")));
	addToDictionary("digit2", createImageResource(getTemplateDesignPath("PhotoTimer\\digit2.png")));
	addToDictionary("digit3", createImageResource(getTemplateDesignPath("PhotoTimer\\digit3.png")));
	addToDictionary("digit4", createImageResource(getTemplateDesignPath("PhotoTimer\\digit4.png")));

	addToDictionary("counts", createImageResource(getTemplateDesignPath("PhotoShooting\\counts.png")));
	addToDictionary("seek", createImageResource(getTemplateDesignPath("PhotoShooting\\seek.png")));
	addToDictionary("plash", createImageResource(getTemplateDesignPath("PhotoShooting\\plash.png")));
	addToDictionary("shootline", createImageResource(getTemplateDesignPath("PhotoShooting\\line.png")));
	addToDictionary("frame", createImageResource(getTemplateDesignPath("PhotoShooting\\frame.png")));

	addToDictionary("print", createImageResource(getTemplateDesignPath("PhotoTemplate\\print.png")));
	addToDictionary("printtitle", createImageResource(getTemplateDesignPath("PhotoTemplate\\title.png")));
	addToDictionary("printline", createImageResource(getTemplateDesignPath("PhotoTemplate\\line.png")));
	addToDictionary("printramka", createImageResource(getTemplateDesignPath("PhotoTemplate\\ramka.png")));


	addToDictionary("galka", createImageResource(getTemplateDesignPath("PhotoChoosing\\galka.png")));
	addToDictionary("ramka", createImageResource(getTemplateDesignPath("PhotoChoosing\\ramka.png")));
	addToDictionary("okBtn", createImageResource(getTemplateDesignPath("PhotoChoosing\\okBtn.png")));
	addToDictionary("reshotBtn", createImageResource(getTemplateDesignPath("PhotoChoosing\\reshotBtn.png")));
	addToDictionary("choosetitle", createImageResource(getTemplateDesignPath("PhotoChoosing\\title.png")));
	addToDictionary("plashFilter", createImageResource(getTemplateDesignPath("PhotoChoosing\\plashFilter.png")));
	
	addToDictionary("chooseTitleFilter", createImageResource(getTemplateDesignPath("PhotoChoosing\\titleFilter.png")));
	addToDictionary("choosefon", createImageResource(getTemplateDesignPath("PhotoChoosing\\fon.png")));

	addToDictionary("again", createImageResource(getTemplateDesignPath("PhotoShare\\again.png")));
	addToDictionary("allApp", createImageResource(getTemplateDesignPath("PhotoShare\\allApp.png")));
	addToDictionary("email", createImageResource(getTemplateDesignPath("PhotoShare\\email.png")));
	addToDictionary("facebook", createImageResource(getTemplateDesignPath("PhotoShare\\facebook.png")));
	addToDictionary("twitter", createImageResource(getTemplateDesignPath("PhotoShare\\twitter.png")));
	addToDictionary("vkontakte", createImageResource(getTemplateDesignPath("PhotoShare\\vkontakte.png")));
	addToDictionary("sharetitle", createImageResource(getTemplateDesignPath("PhotoShare\\title.png")));
	addToDictionary("sharefon", createImageResource(getTemplateDesignPath("PhotoShare\\fon.png")));
	addToDictionary("qrtitle", createImageResource(getTemplateDesignPath("PhotoShare\\qrtitle.png")));
	addToDictionary("bg", createImageResource(getTemplateDesignPath("bg.jpg")));

	addToDictionary("addEmail", createImageResource(getTemplateDesignPath("PhotoShare\\addEmail.png")));
	addToDictionary("enterEmailBorder", createImageResource(getTemplateDesignPath("PhotoShare\\enterEmailBorder.png")));
	addToDictionary("errorEmailBorder", createImageResource(getTemplateDesignPath("PhotoShare\\errorEmailBorder.png")));
	addToDictionary("closePopup", createImageResource(getTemplateDesignPath("PhotoShare\\close.png")));
	addToDictionary("twitterPopupTexture", createImageResource(getTemplateDesignPath("PhotoShare\\twitterPopupTexture.jpg")));

	addToDictionary("error_bg", createImageResource(getTemplateDesignPath("PhotoShare\\error_bg.png")));
	addToDictionary("okPopup", createImageResource(getTemplateDesignPath("PhotoShare\\okPopup.png")));

	smilePaths = fileTools().getAllImagePaths(getTemplateDesignPath("PhotoShooting\\smiles\\"));
	for (size_t i = 0; i < smilePaths.size(); i++)
		addToDictionary("smile" + to_string(i), createImageResource(smilePaths[i]));

	for (auto item : photoOverDesignData)
	{
		addToDictionary(item.getDesignTexName(), createImageResource(getBasePath().string() + item.getDesignPath()));
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));
	}

	for (auto item : photoCardStyles)
	{
		addToDictionary(item.getDesignTexName(), createImageResource(getBasePath().string() + item.getDesignPath()));
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));
	}

	for (auto item : photoFiltersPreview)
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));


	addToSettingsDictionary("arial13", createFontResource(getFontsPath("arial.ttf"), 13));
	addToSettingsDictionary("introBook44", createFontResource(getFontsPath("Intro-Book.ttf"), 44));
	addToSettingsDictionary("helveticaNeueLight24", createFontResource(getFontsPath("Helvetica Neue Light.ttf"), 24));
	addToSettingsDictionary("helvetica40", createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
	addToSettingsDictionary("helvetica100", createFontResource(getFontsPath("Helvetica Neue.ttf"), 100));
	addToSettingsDictionary("helveticaLight24", createFontResource(getFontsPath("HelveticaLight.ttf"), 24));
	addToSettingsDictionary("helveticaNeueLight26", createFontResource(getFontsPath("Helvetica Neue Light.ttf"), 26));

	addToSettingsDictionary("introLight44", createFontResource(getFontsPath("IntroLight.ttf"), 44));
	addToSettingsDictionary("introLight60", createFontResource(getFontsPath("IntroLight.ttf"), 60));
	addToSettingsDictionary("introLight36", createFontResource(getFontsPath("IntroLight.ttf"), 36));
	addToSettingsDictionary("introLight18", createFontResource(getFontsPath("IntroLight.ttf"), 18));
	addToSettingsDictionary("introLight30", createFontResource(getFontsPath("IntroLight.ttf"), 30));
	addToSettingsDictionary("introBook30", createFontResource(getFontsPath("Intro-Book.ttf"), 30));
	addToSettingsDictionary("introBook12", createFontResource(getFontsPath("Intro-Book.ttf"), 12));
	addToSettingsDictionary("introBook14", createFontResource(getFontsPath("Intro-Book.ttf"), 14));
	addToSettingsDictionary("introThin120", createFontResource(getFontsPath("Intro-Thin.ttf"), 120));
	addToSettingsDictionary("introb210", createFontResource(getFontsPath("introb.ttf"), 210));
	addToSettingsDictionary("introb18", createFontResource(getFontsPath("introb.ttf"), 18));
	addToSettingsDictionary("introb21", createFontResource(getFontsPath("introb.ttf"), 21));
}

void PhotoboothSettings::buildSettingData()
{
	logger().log("buildSettingsGameData photobooth");

	auto dic = configTexts.getDic();

	for (auto &it : dic)
		it.second.setFont(fonts);

	configTexts.setDic(dic);

	int i = 0;
	for (auto &it : photoFiltersPreview)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fonts);
		filters[i++].text = it.getTextItem().getText();
	}

	for (auto &it : photoCardStyles)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fonts);
	}

	for (auto &it : photoOverDesignData)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fonts);
	}
}

void PhotoboothSettings::buildLocationData()
{
	logger().log("buildActiveGameData photobooth");
	for (auto &it : photoCardStyles)
		it.setDesignTexture(getTexture(it.getDesignTexName()), photoCardStylesCoordRects);

	for (auto &it : photoOverDesignData)
		it.setDesignTexture(getTexture(it.getDesignTexName()), photoOverCoordRects);

	smileTextures.clear();
	for (size_t i = 0; i < smilePaths.size(); i++)
		smileTextures.push_back(getTexture("smile" + to_string(i)));
};

vector<Texture> PhotoboothSettings::getSmileTextures()
{
	return smileTextures;
}

vector<PhotoboothSettings::Filter> PhotoboothSettings::getOnFilters()
{
	vector<Filter> onFilters;

	for (auto filter : filters)
		if (filter.isOn)
			onFilters.push_back(filter);

	return onFilters;
}

void PhotoboothSettings::swapFilter(int id)
{
	for (auto &filter : filters)
		if (filter.id == id)
			filter.isOn = !filter.isOn;
}

TextItem PhotoboothSettings::getMainTitle(PhtTextID id)
{
	return configTexts.get(model->getLang(), id);
}

TextItem PhotoboothSettings::getSubTitleClose(PhtTextID id)
{
	PhtTextID subID = static_cast<PhtTextID>(int(id) + 1);
	TextItem tItem = configTexts.get(model->getLang(), subID);

	switch (subID)
	{
	case PHOTO_OVER_SUB:
		tItem.setText(getActiveOverDesignText());
		break;
	case CARD_STYLE_SUB:
		tItem.setText(getActiveCardStyleText());
		break;
	case FILTERS_SUB:
		tItem.setText(getActiveFiltersTexts());
		break;
	case PUBLISHING_SUB:
		tItem.setText(getActivePublishingTexts());
		break;
	}

	return tItem;
}

std::string PhotoboothSettings::getActiveOverDesignText()
{
	for (auto item : photoOverDesignData)
		if (item.getID() == activeOverDesignID)
			return item.getTextItem().getText();

	return "";
}

std::string PhotoboothSettings::getActiveCardStyleText()
{
	for (auto item : photoCardStyles)
		if (item.getID() == activePhotoCardStyleDesignID)
			return item.getTextItem().getText();

	return "";
}

std::string PhotoboothSettings::getActiveFiltersTexts()
{
	std::string result = "";
	for (auto item : photoFiltersPreview)
	{
		for (auto filter : filters)
		{
			if (item.getID() == filter.id && filter.isOn)
			{
				if (result.size())
					result += ", ";
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
		if (getSocialState(item))
		{
			if (result.size())
				result += ", ";

			result += getTextItem(item).getText();
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

void PhotoboothSettings::setActiveOverDesignID(int id)
{
	activeOverDesignID = id;
}

int PhotoboothSettings::getUserOverDesignID()
{
	return userOverDesignID;
}

int PhotoboothSettings::getActivePhotoCardStyleDesignID()
{
	return activePhotoCardStyleDesignID;
}

void PhotoboothSettings::setActivePhotoCardStyleDesignID(int id)
{
	activePhotoCardStyleDesignID = id;
}

int PhotoboothSettings::getUserPhotoCardStyleDesignID()
{
	return userPhotoCardStyleDesignID;
}

std::string PhotoboothSettings::getUserPhotoOverDesignPath()
{
	return getBasePath().string() + configPaths.userStickerPath;
}

std::string PhotoboothSettings::getUserPhotoCardStylePath()
{
	return getBasePath().string() + configPaths.userCardStylePath;
}

void PhotoboothSettings::createMemento()
{
	memento = true;
	sharingMemento = sharing;
	filtersMemento = filters;
	activeOverDesignIDMemento = activeOverDesignID;
	activePhotoCardStyleDesignIDMemento = activePhotoCardStyleDesignID;
}

void PhotoboothSettings::writeConfig()
{
	if (memento)
	{
		fs::path basePath(mainConfigObj.getParamsConfigPath());
		JsonTree doc;

		doc.addChild(JsonTree("isFacebook", sharing.getSocialState(PhtTextID::FACEBOOK)));
		doc.addChild(JsonTree("isVkotakte", sharing.getSocialState(PhtTextID::VKONTAKTE)));
		doc.addChild(JsonTree("isTwitter", sharing.getSocialState(PhtTextID::TWITTER)));
		doc.addChild(JsonTree("isEmail", sharing.getSocialState(PhtTextID::EMAIL)));
		doc.addChild(JsonTree("isQrCode", sharing.getSocialState(PhtTextID::QRCODE)));
		doc.addChild(JsonTree("isPrint", sharing.getSocialState(PhtTextID::PRINTER)));

		doc.addChild(JsonTree("isSticker", isSticker));
		doc.addChild(JsonTree("activeOverDesignID", activeOverDesignID));
		doc.addChild(JsonTree("activePhotoCardStyleDesignID", activePhotoCardStyleDesignID));
		doc.addChild(JsonTree("activeSticker", activeSticker.id));
		doc.addChild(JsonTree("activeBgPrint", activeBgPrint.id));

		JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");

		for (auto it : filters)
		{
			JsonTree id;
			id.addChild(JsonTree("id", it.id));
			id.addChild(JsonTree("isOn", it.isOn));
			filtersIdsJ.pushBack(id);
		}

		doc.addChild(filtersIdsJ);
		doc.write(writeFile(basePath), JsonTree::WriteOptions());

		memento = false;
	}
}
bool PhotoboothSettings::settingsChanged()
{
	return (activeOverDesignID != activeOverDesignIDMemento ||
		activePhotoCardStyleDesignID != activePhotoCardStyleDesignIDMemento ||
		sharingNotEqual(sharing, sharingMemento) ||
		filtersNotEqual(filters, filtersMemento));
}

bool PhotoboothSettings::sharingNotEqual(Sharing sharing1, Sharing sharing2)
{
	return (sharing2.getSocialState(PhtTextID::FACEBOOK) != sharing1.getSocialState(PhtTextID::FACEBOOK) ||
		sharing2.getSocialState(PhtTextID::VKONTAKTE) != sharing1.getSocialState(PhtTextID::VKONTAKTE) ||
		sharing2.getSocialState(PhtTextID::TWITTER) != sharing1.getSocialState(PhtTextID::TWITTER) ||
		sharing2.getSocialState(PhtTextID::EMAIL) != sharing1.getSocialState(PhtTextID::EMAIL) ||
		sharing2.getSocialState(PhtTextID::QRCODE) != sharing1.getSocialState(PhtTextID::QRCODE) ||
		sharing2.getSocialState(PhtTextID::PRINTER) != sharing1.getSocialState(PhtTextID::PRINTER));
}

bool PhotoboothSettings::filtersNotEqual(const vector<Filter>& filter1, const vector<Filter>& filter2)
{
	vector<int> f1, f2;

	for (auto filter : filter1)
		if (filter.isOn)
			f1.push_back(filter.id);

	for (auto filter : filter2)
		if (filter.isOn)
			f2.push_back(filter.id);

	std::sort(f1.begin(), f1.end());
	std::sort(f2.begin(), f2.end());

	return (f2 != f1);
}

bool PhotoboothSettings::Filter::isActive() const
{
	return isOn;
}

int PhotoboothSettings::Filter::getID()  const
{
	return id;
}

std::string PhotoboothSettings::Filter::getText() const
{
	return text;
}

std::vector<PhotoboothSettings::Filter> PhotoboothSettings::getFilters() const
{
	return filters;
}

DesignData PhotoboothSettings::getPhotoOverDesignData() const
{
	return photoOverDesignData;
}
DesignData PhotoboothSettings::getPhotoCardStyles() const
{
	return photoCardStyles;
}

DesignData PhotoboothSettings::getPhotoFiltersPreview() const
{
	return photoFiltersPreview;
}

bool PhotoboothSettings::PhotoCountItem::getActive() const
{
	return isActive;
}