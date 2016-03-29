#include "PozaSettings.h"

using namespace kubik;
using namespace kubik::config;

PozaSettings::PozaSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	:ISettings(model), configSettings(configSettings)
{
	mainConfigObj = model->getConfigObject(settings::id::POZA);
}

void PozaSettings::load()
{
	loadPaths();
	setTextures();
}

void PozaSettings::loadPaths()
{
	JsonTree pathJSON = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
	finalPath		  = pathJSON.getChild("finalPath").getValue<string>();
	staticDesignPath  = pathJSON.getChild("interfacePath").getValue<string>();
}

void PozaSettings::setTextures()
{
	setDesignPath();
	clearResources();	
	addToDictionary("title", createImageResource(getTemplateDesignPath("title.png")));
}

void PozaSettings::setDesignPath()
{
	templateDesignPath = configSettings->getTemplateDesign() + finalPath;
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
