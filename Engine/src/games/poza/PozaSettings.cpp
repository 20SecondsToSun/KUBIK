#include "PozaSettings.h"
#include "tools/FileTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

const float PozaSettings::GoToScreenSaverTime = 80.0f;

PozaSettings::PozaSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	StatCollector(configSettings),
	configSettings(configSettings)
{
	mainConfigObj = model->getConfigObject(settings::id::POZA);
}

void PozaSettings::load()
{
	loadPaths();
	loadParams();

	JsonTree designDataJSON = JsonTree(loadFile(getBasePath().string() + pozaCardsConfigPath));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());
		item.setDesignPath(it.getChild("designPath").getValue<string>());

		item.setIconTexName("cardIcon" + to_string(item.getID()));
		item.setDesignTexName("carDesign" + to_string(item.getID()));
		JsonTree text = it.getChild("textObj");

		item.setTextItem(text.getChild("text").getValue<string>(),
			text.getChild("font").getValue<string>(),
			text.getChild("size").getValue<int>(),
			text.getChild("color").getValue<string>());
		photoCardStyles.push_back(item);
	}

	userCardDesignID = designDataJSON.getChild("userDesignID").getValue<int>();
	setTextures();
}   

void PozaSettings::loadPaths()
{
	JsonTree pathJSON	= JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	finalPath			= pathJSON.getChild("finalPath").getValue<string>();
	staticDesignPath	= pathJSON.getChild("interfacePath").getValue<string>();
	pozaDataPath		= pathJSON.getChild("pozaDataPath").getValue<string>();
	pozaBaseFileName	= pathJSON.getChild("pozaBaseFile").getValue<string>();
	pozaCardsConfigPath = pathJSON.getChild("pozaCardsConfigPath").getValue<string>();
}

void PozaSettings::loadParams()
{
	JsonTree paramsJSON = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));
	activeCardDesignID = paramsJSON.getChild("activePhotoCardStyleDesignID").getValue<int>();
}

void PozaSettings::setTextures()
{
	setDesignPath();
	clearResources();	
	addToDictionary("bg", createImageResource(getTemplateDesignPath("bg.jpg")));

	addToDictionary("instrFon", createImageResource(getTemplateDesignPath("intro\\fon.jpg")));

	addToDictionary("handsup", createImageResource(getTemplateDesignPath("handsup\\handsup.png")));
	addToDictionary("man", createImageResource(getTemplateDesignPath("handsup\\man.png")));

	addToDictionary("3meters", createImageResource(getTemplateDesignPath("3meters\\title.png")));
	addToDictionary("arrow", createImageResource(getTemplateDesignPath("3meters\\arrow.png")));

	addToDictionary("circles", createImageResource(getTemplateDesignPath("timer\\circles.png")));
	addToDictionary("title2", createImageResource(getTemplateDesignPath("timer\\title2.png")));

	addToDictionary("_controls", createImageResource(getTemplateDesignPath("game\\controls.png")));
	addToDictionary("controls", createImageResource(getTemplateDesignPath("game\\controls1.png")));
	addToDictionary("good", createImageResource(getTemplateDesignPath("game\\good.png")));
	addToDictionary("bad", createImageResource(getTemplateDesignPath("game\\bad.png")));
	addToDictionary("return", createImageResource(getTemplateDesignPath("game\\return.png")));
	addToDictionary("fail", createImageResource(getTemplateDesignPath("game\\fail.jpg")));
	addToDictionary("totalfail", createImageResource(getTemplateDesignPath("printer\\totalfail.jpg")));
	addToDictionary("printerbg", createImageResource(getTemplateDesignPath("printer\\_printerbg.jpg")));
	addToDictionary("printbtn", createImageResource(getTemplateDesignPath("printer\\printbtn.png")));
	addToDictionary("againbtn", createImageResource(getTemplateDesignPath("printer\\againbtn.png")));
	addToDictionary("othergamesbtn", createImageResource(getTemplateDesignPath("printer\\othergamesbtn.png")));

	addToDictionary("digit1", createImageResource(getTemplateDesignPath("timer\\digit1.png")));
	addToDictionary("digit2", createImageResource(getTemplateDesignPath("timer\\digit2.png")));
	addToDictionary("digit3", createImageResource(getTemplateDesignPath("timer\\digit3.png")));
	addToDictionary("digit4", createImageResource(getTemplateDesignPath("timer\\digit4.png")));

	addToDictionary("sharetitle", createImageResource(getTemplateDesignPath("social\\title.png")));
	addToDictionary("facebook", createImageResource(getTemplateDesignPath("social\\facebook.png")));
	addToDictionary("email", createImageResource(getTemplateDesignPath("social\\email.png")));
	addToDictionary("twitter", createImageResource(getTemplateDesignPath("social\\twitter.png")));
	addToDictionary("vkontakte", createImageResource(getTemplateDesignPath("social\\vkontakte.png")));

	addToDictionary("sharefon", createImageResource(getTemplateDesignPath("social\\fon.png")));
	addToDictionary("qrtitle", createImageResource(getTemplateDesignPath("social\\qrtitle.png")));

	addToDictionary("addEmail", createImageResource(getTemplateDesignPath("social\\addEmail.png")));
	addToDictionary("enterEmailBorder", createImageResource(getTemplateDesignPath("social\\enterEmailBorder.png")));
	addToDictionary("errorEmailBorder", createImageResource(getTemplateDesignPath("social\\errorEmailBorder.png")));
	addToDictionary("closePopup", createImageResource(getTemplateDesignPath("social\\close.png")));
	addToDictionary("popupBg", createImageResource(getTemplateDesignPath("social\\popupBg.png")));

	addToDictionary("successMessage", createImageResource(getTemplateDesignPath("social\\successMessage.png")));
	addToDictionary("errorMessage", createImageResource(getTemplateDesignPath("social\\errorMessage.png")));
	addToDictionary("popupErrorBg", createImageResource(getTemplateDesignPath("social\\popupErrorBg.png")));
	addToDictionary("popupNetralBg", createImageResource(getTemplateDesignPath("social\\popupNetralBg.png")));

	auto path = getBasePath().string() + pozaDataPath + pozaBaseFileName;
	JsonTree pozaData = JsonTree(loadFile(path));
	JsonTree data = JsonTree(pozaData.getChild("poses"));

	for (auto &it : data)
	{
		auto data = it.getChild("data").getValue<string>();
		auto contur = it.getChild("contur").getValue<string>();
		auto comics = it.getChild("comics").getValue<string>();

		PozaDataObj obj(getBasePath().string() + pozaDataPath, data, contur, comics);
		pozaDataVec.push_back(obj);

		addToDictionary(contur, createImageResource(getBasePath().string() + pozaDataPath + contur));
		addToDictionary(comics, createImageResource(getBasePath().string() + pozaDataPath + comics));
	}

	for (auto item : photoCardStyles)
	{
		addToSettingsDictionary(item.getIconTexName(), createImageResource(getInterfacePath(item.getIconPath())));
		addToDictionary(item.getDesignTexName(), createImageResource(getBasePath().string() + item.getDesignPath()));
	}
}

void PozaSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + finalPath;
}

void PozaSettings::buildLocationData()
{
	for (size_t i = 0; i < pozaDataVec.size(); i++)
	{
		auto comicsTex = getTexture(pozaDataVec[i].comics);
		if (comicsTex)
		{
			pozaDataVec[i].comicsTex = comicsTex;
		}
		pozaDataVec[i].conturTex = getTexture(pozaDataVec[i].contur);
	}

	for (auto &it : photoCardStyles)
	{
		auto tex = getTexture(it.getDesignTexName());
		it.setDesignTexture(tex);
	}
}

ci::gl::Texture PozaSettings::getCurrentTemplate()
{
	auto iter = photoCardStyles.begin();
	std::advance(iter, activeCardDesignID - 1);

	return iter->getMappedTextures()[0];
}

void PozaSettings::createMemento()
{

}

void PozaSettings::writeConfig()
{

}

bool PozaSettings::settingsChanged()
{
	return false;
}

changeSetting::id PozaSettings::getChangeID() const
{
	return changeSetting::id::POZA;
}

std::string PozaSettings::getDataBasePath() const
{
	return configSettings->getActionName() + "\\poza\\";
}

PozaSettings::PozaDataObj::PozaDataObj()
{

}

PozaSettings::PozaDataObj::PozaDataObj(const std::string& path, const std::string& data, const std::string& contur, const std::string& comics)
{
	this->path   = path;
	this->data   = data;
	this->contur = contur;
	this->comics = comics;
	auto _path = fs::path(const_cast<string&>(path)+"\\" + const_cast<string&>(data));
	logger().log("_path :: " + _path.string());
	JsonTree doc = JsonTree(loadFile(_path));
	JsonTree poses(doc.getChild("poses"));


	for (auto pose = poses.begin(); pose != poses.end(); ++pose)
	{
		//vector<Vec2f> points;

		JsonTree datas = JsonTree(pose->getChild("data"));
		for (JsonTree::ConstIter data = datas.begin(); data != datas.end(); ++data)
		{
			float x = data->getChild("x").getValue<float>();
			float y = data->getChild("y").getValue<float>();
			logger().log("x :: " + toString(x) + "  y :: " + toString(y));
			points.push_back(Vec2f(x, y));
		}
	}
}
