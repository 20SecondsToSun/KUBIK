#include "PhotoboothSettings.h"
#include "dataBase/DataBase.h"

using namespace kubik;
using namespace kubik::config;
using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

const float PhotoboothSettings::GoToScreenSaverTime = 80.0f;
const int PhotoboothSettings::PhotoSeconds = 5;

PhotoboothSettings::PhotoboothSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	StatCollector(configSettings),
	configSettings(configSettings),
	memento(false)
{
	
}

void PhotoboothSettings::load()
{
	logger().log("~~~ PhotoboothSettings.StartLoad ~~~");

	mainConfigObj = model->getConfigObject(settings::id::PHOTOBOOTH);

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
	JsonTree pathJSON							  = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	configPaths.userStickerPath					  = pathJSON.getChild("userStickerPath").getValue<string>();
	configPaths.userCardStylePath				  = pathJSON.getChild("userCardStylePath").getValue<string>();
	configPaths.photoOverDesignDataPath			  = pathJSON.getChild("photoOverDesignDataPath").getValue<string>();
	configPaths.photoCardsStylesDesignDataPath    = pathJSON.getChild("photoCardsStylesPath").getValue<string>();
	configPaths.photoFiltersPreviewDesignDataPath = pathJSON.getChild("photoFiltersPreviewDesignDataPath").getValue<string>();
	configPaths.finalPath						  = pathJSON.getChild("finalPath").getValue<string>();
	staticDesignPath							  = pathJSON.getChild("interfacePath").getValue<string>();
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
	loadSharingIcons(constsJSON);
}

void PhotoboothSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + configPaths.finalPath;
}

void PhotoboothSettings::loadSocialParams(const JsonTree& config)
{
	setSocialState(PhtTextID::FACEBOOK,  config.getChild("isFacebook").getValue<bool>());
	setSocialState(PhtTextID::VKONTAKTE, config.getChild("isVkotakte").getValue<bool>());
	setSocialState(PhtTextID::TWITTER,   config.getChild("isTwitter").getValue<bool>());
	setSocialState(PhtTextID::EMAIL,     config.getChild("isEmail").getValue<bool>());
	setSocialState(PhtTextID::QRCODE,    config.getChild("isQrCode").getValue<bool>());
	setSocialState(PhtTextID::PRINTER,   config.getChild("isPrint").getValue<bool>());
}

void PhotoboothSettings::loadPhotoFilterParams(const JsonTree& config)
{
	JsonTree datas = JsonTree(config.getChild("filtersIds"));

	for (auto it : datas)
	{
		Filter filter;
		filter.id	= it.getChild("id").getValue<int>();
		filter.isOn = it.getChild("isOn").getValue<bool>();
		filters.push_back(filter);
	}
}

void PhotoboothSettings::loadGameDesignParams(const JsonTree& config)
{
	activeOverDesignID			 = config.getChild("activeOverDesignID").getValue<int>();
	isSticker					 = config.getChild("isSticker").getValue<bool>();
	activeSticker.id			 = config.getChild("activeSticker").getValue<int>();
	activePhotoCardStyleDesignID = config.getChild("activePhotoCardStyleDesignID").getValue<int>();
	activeBgPrint.id			 = config.getChild("activeBgPrint").getValue<int>();
}

void PhotoboothSettings::loadConfigTexts(const JsonTree& config)
{
	JsonTree jsonTexts = JsonTree(config.getChild("mainTitles"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::CARD_STYLE, jtools().parseTextItem(it.getChild("photoCardDesign")));
		configTexts.insert(lang, PhtTextID::PHOTO_OVER, jtools().parseTextItem(it.getChild("overElements")));
		configTexts.insert(lang, PhtTextID::FILTERS,    jtools().parseTextItem(it.getChild("photoFilters")));
		configTexts.insert(lang, PhtTextID::PUBLISHING, jtools().parseTextItem(it.getChild("photoPublishing")));
	}

	jsonTexts = JsonTree(config.getChild("subTitles"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::CARD_STYLE_SUB, jtools().parseTextItem(it.getChild("photoCardDesign")));
		configTexts.insert(lang, PhtTextID::PHOTO_OVER_SUB, jtools().parseTextItem(it.getChild("overElements")));
		configTexts.insert(lang, PhtTextID::FILTERS_SUB,    jtools().parseTextItem(it.getChild("photoFilters")));
		configTexts.insert(lang, PhtTextID::PUBLISHING_SUB, jtools().parseTextItem(it.getChild("photoPublishing")));
	}

	jsonTexts = JsonTree(config.getChild("social"));
	for (auto it : jsonTexts)
	{
		string lang = it.getChild("lang").getValue<string>();
		configTexts.insert(lang, PhtTextID::EMAIL,     jtools().parseTextItem(it.getChild("email")));
		configTexts.insert(lang, PhtTextID::PRINTER,   jtools().parseTextItem(it.getChild("printer")));
		configTexts.insert(lang, PhtTextID::QRCODE,    jtools().parseTextItem(it.getChild("qr")));
		configTexts.insert(lang, PhtTextID::TWITTER,   jtools().parseTextItem(it.getChild("tw")));
		configTexts.insert(lang, PhtTextID::VKONTAKTE, jtools().parseTextItem(it.getChild("vk")));
		configTexts.insert(lang, PhtTextID::FACEBOOK,  jtools().parseTextItem(it.getChild("fb")));
	}
}

void PhotoboothSettings::loadSharingIcons(const JsonTree& config)
{
	typedef Pair<PhtTextID, string> NamePair;

	vector<NamePair> pairs;
	pairs.push_back(NamePair(PhtTextID::FACEBOOK,  "fbIcon"));
	pairs.push_back(NamePair(PhtTextID::PRINTER,   "printerIcon"));
	pairs.push_back(NamePair(PhtTextID::VKONTAKTE, "vkIcon"));
	pairs.push_back(NamePair(PhtTextID::TWITTER,   "twIcon"));
	pairs.push_back(NamePair(PhtTextID::QRCODE,    "qrIcon"));
	pairs.push_back(NamePair(PhtTextID::EMAIL,     "emailIcon"));

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
		stickersDesignData.push_back(item);
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
		cardsBackgroundDesignData.push_back(item);
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

void PhotoboothSettings::setTextures()
{
	setDesignPath();
	clearResources();

	addToSettingsDictionary("over6", createImageResource(getInterfacePath("configDesign\\photobooth\\over6.png")));

	addToDictionary("instrFon",   createImageResource(getTemplateDesignPath("PhotoInstruction\\screensaver\\1.jpg")));
	addToDictionary("instrTitle", createImageResource(getTemplateDesignPath("PhotoInstruction\\title\\title.png")));
	addToDictionary("filterTitle",createImageResource(getTemplateDesignPath("PhotoFilter\\title.png")));
	addToDictionary("timer1",     createImageResource(getTemplateDesignPath("PhotoTimer\\timer1.png")));
	addToDictionary("timer2",     createImageResource(getTemplateDesignPath("PhotoTimer\\timer2.png")));
	addToDictionary("timertitle", createImageResource(getTemplateDesignPath("PhotoTimer\\title.png")));
	addToDictionary("digit1",	  createImageResource(getTemplateDesignPath("PhotoTimer\\digit1.png")));
	addToDictionary("digit2",	  createImageResource(getTemplateDesignPath("PhotoTimer\\digit2.png")));
	addToDictionary("digit3",	  createImageResource(getTemplateDesignPath("PhotoTimer\\digit3.png")));
	addToDictionary("digit4",	  createImageResource(getTemplateDesignPath("PhotoTimer\\digit4.png")));

	addToDictionary("counts", createImageResource(getTemplateDesignPath("PhotoShooting\\counts.png")));
	addToDictionary("seek", createImageResource(getTemplateDesignPath("PhotoShooting\\seek.png")));
	addToDictionary("plash", createImageResource(getTemplateDesignPath("PhotoShooting\\plash.png")));
	addToDictionary("shootline", createImageResource(getTemplateDesignPath("PhotoShooting\\line.png")));
	addToDictionary("frame", createImageResource(getTemplateDesignPath("PhotoShooting\\frame.png")));
	addToDictionary("shadow", createImageResource(getTemplateDesignPath("PhotoShooting\\shadow.png")));

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
	addToDictionary("successMessage", createImageResource(getTemplateDesignPath("PhotoShare\\successMessage.png")));
	addToDictionary("errorMessage", createImageResource(getTemplateDesignPath("PhotoShare\\errorMessage.png")));

	addToDictionary("bg", createImageResource(getTemplateDesignPath("bg.jpg")));
	addToDictionary("popupNetralBg", createImageResource(getTemplateDesignPath("popupNetralBg.png")));
	addToDictionary("popupErrorBg", createImageResource(getTemplateDesignPath("popupErrorBg.png")));
	addToDictionary("cameraErrorText", createImageResource(getTemplateDesignPath("cameraErrorText.png")));

	addToDictionary("popupBg", createImageResource(getTemplateDesignPath("PhotoShare\\popupBg.png")));
	addToDictionary("addEmail", createImageResource(getTemplateDesignPath("PhotoShare\\addEmail.png")));
	addToDictionary("enterEmailBorder", createImageResource(getTemplateDesignPath("PhotoShare\\enterEmailBorder.png")));
	addToDictionary("errorEmailBorder", createImageResource(getTemplateDesignPath("PhotoShare\\errorEmailBorder.png")));
	addToDictionary("closePopup", createImageResource(getTemplateDesignPath("PhotoShare\\close.png")));
	addToDictionary("twitterPopupTexture", createImageResource(getTemplateDesignPath("PhotoShare\\twitterPopupTexture.jpg")));
	addToDictionary("error_bg", createImageResource(getTemplateDesignPath("PhotoShare\\error_bg.png")));
	addToDictionary("okPopup", createImageResource(getTemplateDesignPath("PhotoShare\\okPopup.png")));

	{// load smiles
		smilePaths = fileTools().getAllImagePaths(getTemplateDesignPath("PhotoShooting\\smiles\\"));
		for (size_t i = 0; i < smilePaths.size(); i++)
		{
			addToDictionary("smile" + to_string(i), createImageResource(smilePaths[i]));
		}			
	}	

	{// load stickers photo designs
		for (size_t i = 0; i < stickersDesignData.size(); i++)
		{
			auto sticker = stickersDesignData[i];
			addToSettingsDictionary(sticker.getIconTexName(), createImageResource(getInterfacePath(sticker.getIconPath())));

			for (size_t j = 1; j <= STICKERS_COUNT; j++)
			{
				auto stickerSubName = "sticker" + to_string(i + 1) + "_" + to_string(j);
				auto stickerSubPath = getBasePath().string() + sticker.getDesignPath() + "\\" + to_string(j) + ".png";		
				addToDictionary(stickerSubName, createImageResource(stickerSubPath));
			}			
		}	
	}

	{// load backgrounds photo designs

		for (size_t i = 0; i < cardsBackgroundDesignData.size(); i++)
		{
			auto photoCard = cardsBackgroundDesignData[i];
			addToSettingsDictionary(photoCard.getIconTexName(), createImageResource(getInterfacePath(photoCard.getIconPath())));

			for (size_t j = 1; j <= CARDS_COUNT; j++)
			{				
				addToDictionary(getPhotoCardName(i + 1, j), createImageResource(getPhotoCardPath(photoCard, j)));
			}
		}
	}	
	
	for (auto item : photoFiltersPreview)
	{
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));
	}	
}

void PhotoboothSettings::buildSettingData()
{	
	for (auto &it : configTexts.getDic())
	{
		it.second.setFont(fontStorage().getAll());
	}		

	int i = 0;
	for (auto &it : photoFiltersPreview)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fontStorage().getAll());
		filters[i++].text = it.getTextItem().getText();
	}

	for (auto &it : cardsBackgroundDesignData)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fontStorage().getAll());
	}

	for (auto &it : stickersDesignData)
	{
		it.setIcon(getTexture(it.getIconTexName()));
		it.setFont(fontStorage().getAll());
	}	
}

void PhotoboothSettings::buildLocationData()
{
	photoCardsImagesMap.clear();
	for (size_t i = 0; i < cardsBackgroundDesignData.size(); i++)
	{
		for (size_t j = 0; j < CARDS_COUNT; j++)
		{
			PhotoCardKey Key = std::make_pair(i, PhotoFormat(j));
			auto photoCardSubName = "photoCard" + to_string(i + 1) + "_" + to_string(j + 1);
			auto Pair = std::make_pair(Key, getTexture(photoCardSubName));
			photoCardsImagesMap.insert(Pair);
		}
	}

	stickersImagesMap.clear();
	for (size_t i = 0; i < stickersDesignData.size(); i++)
	{
		for (size_t j = 0; j < STICKERS_COUNT; j++)
		{
			StickerKey Key = std::make_pair(i, PhotoFormat(j));
			auto stickerSubName = "sticker" + to_string(i + 1) + "_" + to_string(j+1);
			auto Pair = std::make_pair(Key, getTexture(stickerSubName));
			stickersImagesMap.insert(Pair);
		}
	}	

	smileTextures.clear();
	for (size_t i = 0; i < smilePaths.size(); i++)
	{
		smileTextures.push_back(getTexture("smile" + to_string(i)));
	}
}

vector<Texture> PhotoboothSettings::getSmileTextures() const
{
	return smileTextures;
}

std::vector<ci::gl::Texture> PhotoboothSettings::getStickerTextures() const
{
	std::vector<ci::gl::Texture> temp;

	for (size_t i = 0; i < STICKERS_COUNT; i++)
	{
		auto Pair = std::make_pair(activeOverDesignID-1, PhotoFormat(i));
		temp.push_back(stickersImagesMap.at(Pair));
	}	

	return temp;
}

std::vector<ci::gl::Texture> PhotoboothSettings::getPhotoCardsTextures() const
{
	std::vector<ci::gl::Texture> temp;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		auto Pair = std::make_pair(activePhotoCardStyleDesignID - 1, PhotoFormat(i));
		temp.push_back(photoCardsImagesMap.at(Pair));
	}

	return temp;
}

Texture PhotoboothSettings::getPhotoShootingCard() const
{
	auto Pair = std::make_pair(activePhotoCardStyleDesignID - 1, PhotoFormat(1));
	return photoCardsImagesMap.at(Pair);
}

Texture PhotoboothSettings::getPhotoSharingCard() const
{
	auto Pair = std::make_pair(activePhotoCardStyleDesignID - 1, PhotoFormat(1));
	return photoCardsImagesMap.at(Pair);
}

Texture PhotoboothSettings::getPhotoSharingSticker() const
{
	auto Pair = std::make_pair(activeOverDesignID - 1, PhotoFormat(1));
	return stickersImagesMap.at(Pair);
}



vector<PhotoboothSettings::Filter> PhotoboothSettings::getOnFilters()
{
	vector<Filter> onFilters;

	for (auto filter : filters)
	{
		if (filter.isOn)
		{
			onFilters.push_back(filter);
		}			
	}	

	return onFilters;
}

void PhotoboothSettings::swapFilter(int id)
{
	for (auto &filter : filters)
	{
		if (filter.id == id)
		{
			filter.isOn = !filter.isOn;
		}			
	}		
}

TextItem PhotoboothSettings::getMainTitle(const PhtTextID& id)
{
	return configTexts.get(model->getLang(), id);
}

TextItem PhotoboothSettings::getSubTitleClose(const PhtTextID& id)
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
	for (auto item : stickersDesignData)
	{
		if (item.getID() == activeOverDesignID)
		{
			return item.getTextItem().getText();
		}			
	}	

	return "";
}

std::string PhotoboothSettings::getActiveCardStyleText()
{
	for (auto item : cardsBackgroundDesignData)
	{
		if (item.getID() == activePhotoCardStyleDesignID)
		{
			return item.getTextItem().getText();
		}			
	}		

	return "";
}

std::string PhotoboothSettings::getActiveFiltersTexts()
{
	std::string result = "";
	std::string delimeter = ",";

	for (auto item : photoFiltersPreview)
	{
		for (auto filter : filters)
		{
			if (item.getID() == filter.id && filter.isOn)
			{
				if (result.size())
				{
					result += delimeter;
				}
					
				result += item.getTextItem().getText();
			}
		}
	}

	return result;
}

std::string PhotoboothSettings::getActivePublishingTexts()
{
	std::string result = "";
	std::string delimeter = ",";

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
			{
				result += delimeter;
			}

			result += getTextItem(item).getText();
		}
	}		

	return result;
}

TextItem PhotoboothSettings::getSubTitleOpen(const PhtTextID& id)
{
	return configTexts.get(model->getLang(), static_cast<PhtTextID>(int(id) + 1));
}

TextItem PhotoboothSettings::getTextItem(const PhtTextID& id)
{
	return configTexts.get(model->getLang(), id);
}

bool PhotoboothSettings::getSocialState(const PhtTextID& id)
{
	return sharing.getSocialState(id);
}

void PhotoboothSettings::setSocialState(const PhtTextID& id, bool value)
{
	sharing.setSocialState(id, value);
}

ci::gl::Texture PhotoboothSettings::getIcon(const PhtTextID& id)
{
	return sharing.getIcon(id);
}

ci::gl::Texture PhotoboothSettings::getEmptyIcon() const
{
	return sharing.getEmptyIcon();
}

void PhotoboothSettings::Sharing::setSocialState(const PhtTextID& id, bool state)
{
	states[id] = state;
}

bool PhotoboothSettings::Sharing::getSocialState(const PhtTextID& id)
{
	return states[id];
}

void PhotoboothSettings::Sharing::setIcon(const ci::gl::Texture& icon, const PhtTextID& id)
{
	icons[id] = icon;
}

ci::gl::Texture PhotoboothSettings::Sharing::getIcon(const PhtTextID& id)
{
	return icons[id];
}

void PhotoboothSettings::Sharing::setEmptyIcon(const ci::gl::Texture& icon)
{
	emptyIcon = icon;
}

ci::gl::Texture PhotoboothSettings::Sharing::getEmptyIcon() const
{
	return emptyIcon;
}

int PhotoboothSettings::getActiveOverDesignID() const
{
	return activeOverDesignID;
}

void PhotoboothSettings::setActiveOverDesignID(int id)
{
	activeOverDesignID = id;
}

int PhotoboothSettings::getUserOverDesignID() const
{
	return userOverDesignID;
}

int PhotoboothSettings::getActivePhotoCardStyleDesignID() const
{
	return activePhotoCardStyleDesignID;
}

void PhotoboothSettings::setActivePhotoCardStyleDesignID(int id)
{
	activePhotoCardStyleDesignID = id;
}

int PhotoboothSettings::getUserPhotoCardStyleDesignID() const
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

IMovieRef PhotoboothSettings::getPreloader()
{
	return IMovieRef();
}

bool PhotoboothSettings::isPrinterOn()
{
	return sharing.getSocialState(PhtTextID::PRINTER);
}

bool PhotoboothSettings::onlyOneGameOn()
{
	return model->onlyOneGameOn();
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
		doc.addChild(JsonTree("isTwitter",  sharing.getSocialState(PhtTextID::TWITTER)));
		doc.addChild(JsonTree("isEmail",    sharing.getSocialState(PhtTextID::EMAIL)));
		doc.addChild(JsonTree("isQrCode",   sharing.getSocialState(PhtTextID::QRCODE)));
		doc.addChild(JsonTree("isPrint",    sharing.getSocialState(PhtTextID::PRINTER)));

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
		sharing2.getSocialState(PhtTextID::VKONTAKTE)	 != sharing1.getSocialState(PhtTextID::VKONTAKTE) ||
		sharing2.getSocialState(PhtTextID::TWITTER)		 != sharing1.getSocialState(PhtTextID::TWITTER) ||
		sharing2.getSocialState(PhtTextID::EMAIL)		 != sharing1.getSocialState(PhtTextID::EMAIL) ||
		sharing2.getSocialState(PhtTextID::QRCODE)		 != sharing1.getSocialState(PhtTextID::QRCODE) ||
		sharing2.getSocialState(PhtTextID::PRINTER)		 != sharing1.getSocialState(PhtTextID::PRINTER));
}

bool PhotoboothSettings::filtersNotEqual(const vector<Filter>& filter1, const vector<Filter>& filter2)
{
	vector<int> f1, f2;

	for (auto filter : filter1)
	{
		if (filter.isOn)
		{
			f1.push_back(filter.id);
		}			
	}		

	for (auto filter : filter2)
	{
		if (filter.isOn)
		{
			f2.push_back(filter.id);
		}			
	}		

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
	return stickersDesignData;
}

DesignData PhotoboothSettings::getPhotoCardStyles() const
{
	return cardsBackgroundDesignData;
}

DesignData PhotoboothSettings::getPhotoFiltersPreview() const
{
	return photoFiltersPreview;
}

bool PhotoboothSettings::PhotoCountItem::getActive() const
{
	return isActive;
}

std::string PhotoboothSettings::getPhotoCardName(int i, int j) const
{
	return "photoCard" + to_string(i) + "_" + to_string(j);
}

std::string PhotoboothSettings::getPhotoCardPath(const OneDesignItem& photoCard, int j) const
{
	return getBasePath().string() + photoCard.getDesignPath() + "\\" + to_string(j) + ".png";
}

std::string PhotoboothSettings::getDataBasePath() const
{
	return configSettings->getActionName() + "\\photobooth\\";
}

std::string PhotoboothSettings::getDataBaseName() const
{
	auto timeStruct = Utils::getCurrentTime();

	return Utils::fix2(to_string(timeStruct.tm_mday))	 + "_"
		 + Utils::fix2(to_string(timeStruct.tm_mon + 1)) + "_"
		 + to_string(1900 + timeStruct.tm_year)			 + ".csv";
}

void PhotoboothSettings::saveStatData(const std::shared_ptr<DataBaseRecord>& db)
{
	std::string basePath = getDataBasePath();
	std::string baseName = getDataBaseName();
	auto fullDirPath	 = Paths::getDataBasePath(basePath);

	static const string delimeter = ";";
	if (Paths::createIfDoesntExist(fullDirPath, baseName))
	{
		data_base().saveData(fullDirPath + baseName, db->getRecordTitle());
	}	

	data_base().saveData(fullDirPath + baseName, db->getRecord());
}

