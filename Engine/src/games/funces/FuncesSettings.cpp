#include "FuncesSettings.h"

using namespace ci;
using namespace std;
using namespace kubik;
using namespace kubik::config;

FuncesSettings::FuncesSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model),
	configSettings(configSettings)
{
	mainConfigObj = model->getConfigObject(settings::id::FUNCES);
}

void FuncesSettings::load()
{
	loadPaths();
	setTextures();
}

void FuncesSettings::loadPaths()
{
	JsonTree pathJSON = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	finalPath         = pathJSON.getChild("finalPath").getValue<string>();
	staticDesignPath  = pathJSON.getChild("interfacePath").getValue<string>();
}

void FuncesSettings::setTextures()
{
	setDesignPath();
	clearResources();

	addToDictionary("title", createImageResource(getTemplateDesignPath("title.png")));
}

void FuncesSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + finalPath;
}

void FuncesSettings::createMemento()
{

}

void FuncesSettings::writeConfig()
{

}

bool FuncesSettings::settingsChanged()
{
	return false;
}

changeSetting::id FuncesSettings::getChangeID() const
{
	return changeSetting::id::FUNCES;
}
