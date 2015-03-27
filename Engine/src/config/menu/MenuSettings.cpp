#include "MenuSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

MenuSettings::MenuSettings(ApplicationModelRef model):ISettings(model)
{
	mainConfigPath = model->getMenuConfigPath();
	//load();		
}

void MenuSettings::load()
{
	logger().log("menu settings load");

	JsonTree configJSON = JsonTree(loadFile(mainConfigPath));
	data.staticPartDesignPath = configJSON.getChild("staticPartDesignPath").getValue<string>();
	data.kubikTemplatePartDesignPath = configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();
	data.userTemplatePartDesignPath = configJSON.getChild("userTemplatePartDesignPath").getValue<string>();
	data.finalPath = configJSON.getChild("finalPath").getValue<string>();
	data.templateId = configJSON.getChild("templateId").getValue<int>();
	data.isCustomDesign = configJSON.getChild("isCustomDesign").getValue<bool>();

	setDesignPath();
	setTextures();
}

void MenuSettings::saveConfig()
{
	console() << "SAVE MENU CONFIG" << endl;

	fs::path basePath(mainConfigPath);

	JsonTree doc;
	doc.addChild(JsonTree("staticPartDesignPath", data.staticPartDesignPath));
	doc.addChild(JsonTree("kubikTemplatePartDesignPath", data.kubikTemplatePartDesignPath));
	doc.addChild(JsonTree("userTemplatePartDesignPath", data.userTemplatePartDesignPath));
	doc.addChild(JsonTree("finalPath", data.finalPath));
	doc.addChild(JsonTree("templateId", data.templateId));
	doc.addChild(JsonTree("isCustomDesign", data.isCustomDesign));
	doc.write(writeFile(basePath), JsonTree::WriteOptions());
}

bool MenuSettings::settingsChanged()
{
	return false; 
};

changeSetting::id MenuSettings::getChangeID() const
{ 
	return changeSetting::id::MENU;
};

void MenuSettings::setDesignPath()
{
	if (data.isCustomDesign)
		templateDesignPath = data.userTemplatePartDesignPath + to_string(data.templateId) + "\\" + data.finalPath;
	else
		templateDesignPath = data.kubikTemplatePartDesignPath + to_string(data.templateId) + "\\" + data.finalPath;

	staticDesignPath = data.staticPartDesignPath + data.finalPath;
}

void MenuSettings::setTextures()
{
	clearResources();
	addToDictionary("background", createImageResource(getTemplateDesignPath("bg.png")));
	addToDictionary("menuButton", createImageResource(getTemplateDesignPath("menulayer.png")));
	addToDictionary("helvetica30", createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
}

void MenuSettings::setData(MenuDataStruct value)
{
	data = value;
	saveConfig();
}

MenuSettings::MenuDataStruct MenuSettings::getData()
{
	return data;
}

vector<GameData> MenuSettings::getEnabledGamesData()
{
	auto games =  model->getGames();
	vector<GamesInfo> filtergames;

	for(auto it : games)	
		if (it.isOn && it.isPurchased)
		{
			filtergames.push_back(it);
			console() << "id:::::::::::::  " << it.id << endl;
		}
			

	vector<GameData> gameData;
	vector<Vec2f> position;
	Vec2i size;

	switch (filtergames.size())
	{
	case 1:
		return gameData;	
	case 2:
		size = Vec2i(518, 518);
		position.push_back(Vec2f((getWindowWidth() - 518.0f) * 0.5f, 179.0f));
		position.push_back(Vec2f((getWindowWidth() - 518.0f) * 0.5f, 1017.0f));		
		break;
	case 3:
		size = Vec2i(305, 305);
		position.push_back(Vec2f(165.0f, 239.0f));
		position.push_back(Vec2f(165.0f, 731.0f));
		position.push_back(Vec2f(165.0f, 1243.0f));
		break;
	case 4:
		size = Vec2i(305, 305);
		position.push_back(Vec2f(165.0f, 239.0f));
		position.push_back(Vec2f(165.0f, 731.0f));
		position.push_back(Vec2f(165.0f, 1243.0f));
		position.push_back(Vec2f(165.0f, 1243.0f));
		break;
	}
			
	for (unsigned int i = 0; i < filtergames.size(); i++)
	{
		GameData oneGame;
		oneGame.id = filtergames[i].getGameId();
		oneGame.position = position[i];

		Texture tex = filtergames[i].getTexture();
		float scale = (float)size.x / tex.getWidth();
	
		oneGame.texture = Utils::drawGraphicsToFBO(size, [&]()
		{ 			
			gl::scale(scale, scale);
			gl::draw(tex);
		});
		gameData.push_back(oneGame);
	}

	return gameData;
}

vector<GamesInfo> MenuSettings::getGames()
{
	return model->getGames();
}
//
//string MenuSettings::getUserDesighFullPath()
//{
//	return getBasePath().string() + data.userTemplatePartDesignPath + to_string(data.templateId) + "\\" + data.finalPath;
//}
//
//string MenuSettings::getKubikDesighFullPath()
//{
//	return getBasePath().string() + data.kubikTemplatePartDesignPath + to_string(data.templateId) + "\\" + data.finalPath;
//}