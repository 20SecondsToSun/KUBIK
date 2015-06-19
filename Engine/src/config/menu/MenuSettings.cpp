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

	addToDictionary("2gamesBG", createImageResource(getTemplateDesignPath("2games\\buttonBg.png")));
	addToDictionary("2gamesTitle1", createImageResource(getTemplateDesignPath("2games\\title1.png")));
	addToDictionary("2gamesTitle2", createImageResource(getTemplateDesignPath("2games\\title2.png")));
	addToDictionary("2gamesTitle3", createImageResource(getTemplateDesignPath("2games\\title3.png")));
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

AdditionalGameData MenuSettings::getMenuScreenAdditionalDesignElements()
{
	auto games = model->getGames();
	vector<GamesInfo> filtergames;

	for (auto it : games)
		if (it.isOn && it.isPurchased)
			filtergames.push_back(it);

	AdditionalGameData gamedata;

	if (!filtergames.empty())
	{
		string prefix = to_string(filtergames.size()) + "games";	
		gamedata.setBackground(getTexture(prefix + "BG"));

		Vec2f bgPosition, titlePosition;

		switch (filtergames.size())
		{
		case 2:
			bgPosition = Vec2f(-240.0f, -100.0f);
			titlePosition = Vec2f(28.0f, 590.0f);
			break;

		case 3:
			bgPosition = Vec2f(-240.0f, -100.0f);
			break;
		}

		for (size_t i = 0; i < filtergames.size(); i++)				
			gamedata.addTitle(filtergames[i].getGameId(), getTexture(prefix + "Title" + to_string(filtergames[i].getGameId())));			
		
		gamedata.setTitlePosition(titlePosition);
		gamedata.setBackgroundPosition(bgPosition);
	}

	return gamedata;
}

vector<GameData> MenuSettings::getEnabledGamesData()
{
	auto games =  model->getGames();
	vector<GamesInfo> filtergames;

	for(auto it : games)	
		if (it.isOn && it.isPurchased)		
			filtergames.push_back(it);			

	vector<GameData> gameData;
	vector<Vec2f> position;
	Vec2i size;

	switch (filtergames.size())
	{
	case 1:
		return gameData;	

	case 2:
		size = Vec2i(518, 518);
		position.push_back(Vec2f((getWindowWidth() - 518.0f) * 0.5f, 140.0f));
		position.push_back(Vec2f((getWindowWidth() - 518.0f) * 0.5f, 1094.0f));		
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

bool MenuSettings::MenuDataStruct::hasDesignChanges(const MenuDataStruct& menu)
{
	return (isCustomDesign != menu.isCustomDesign ||
		templateId != menu.templateId);
}

ci::gl::Texture GameData::getTexture() const
{
	return texture;
}

ci::Vec2f GameData::getPosition() const
{
	return position;
}

GameId GameData::getID() const
{
	return id;
}

ci::gl::Texture AdditionalGameData::getBackground() const
{
	return background;
}

void AdditionalGameData::setBackground(const ci::gl::Texture& texture)
{
	background = texture;
}

void AdditionalGameData::addTitle(const GameId& value, const ci::gl::Texture& texture)
{
	titles[value] = texture;
}

ci::Vec2f AdditionalGameData::getBackgroundPosition() const
{
	return backgroundPosition;
}

ci::Vec2f AdditionalGameData::getTitlePosition() const
{
	return titlePosition;
}

void AdditionalGameData::setBackgroundPosition(const ci::Vec2f& value)
{
	backgroundPosition = value;
}

void AdditionalGameData::setTitlePosition(const ci::Vec2f& value)
{
	titlePosition = value;
}

ci::gl::Texture AdditionalGameData::getTitleByID(const GameId& value)
{
	return titles[value];
}