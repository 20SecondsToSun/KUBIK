#include "StatCollector.h"
#include "database/DataBase.h"

using namespace kubik::config;
using namespace kubik;
using namespace std;

const std::string StatCollector::DELIMETER = ";";

StatCollector::StatCollector(ConfigSettingsRef configSettings)
	:configSettings(configSettings)
{

}

void StatCollector::addPlayedGame(int count)
{
	configSettings->addPlayedCount(count);
	configSettings->writeConfigForce();
}

void StatCollector::addPrintedCount()
{
	configSettings->addPrintedCount();
	configSettings->writeConfigForce();
}

void StatCollector::addFBShare()
{
	configSettings->addPuplishedCount();
	configSettings->writeConfigForce();
}

void StatCollector::addVKShare()
{
	configSettings->addPuplishedCount();
	configSettings->writeConfigForce();
}

void StatCollector::addTWShare()
{
	configSettings->addPuplishedCount();
	configSettings->writeConfigForce();
}

void StatCollector::addEmailShare()
{
	configSettings->addPuplishedCount();
	configSettings->writeConfigForce();
}

std::string StatCollector::getDatePrefix() const
{
	using namespace std;
	auto timeStruct = Utils::getCurrentTime();

	return Utils::fix2(to_string(timeStruct.tm_mday)) + "_"
		 + Utils::fix2(to_string(timeStruct.tm_mon + 1)) + "_"
		 + to_string(1900 + timeStruct.tm_year);
}

std::string StatCollector::getDBExt() const
{
	 return ".csv";
}

std::string StatCollector::getTimeFormat() const
{
	using namespace std;
	auto timeStruct = Utils::getCurrentTime();

	return  Utils::fix2(to_string(timeStruct.tm_hour)) + ":"
		  + Utils::fix2(to_string(timeStruct.tm_min)) + ":"
		  + Utils::fix2(to_string(timeStruct.tm_sec));
}

void StatCollector::saveStatData(const std::shared_ptr<DataBaseRecord>& db, const std::string& basePath, const std::string& baseName)
{
	auto fullDirPath = Paths::getDataBasePath(basePath);

	static const std::string delimeter = ";";
	if (Paths::createIfDoesntExist(fullDirPath, baseName))
	{
		data_base().saveData(fullDirPath + baseName, db->getRecordTitle());
	}

	data_base().saveData(fullDirPath + baseName, db->getRecord());
}

std::string StatCollector::getDataBaseName() const
{
	auto timeStruct = Utils::getCurrentTime();

	return Utils::fix2(to_string(timeStruct.tm_mday)) + "_"
		+ Utils::fix2(to_string(timeStruct.tm_mon + 1)) + "_"
		+ to_string(1900 + timeStruct.tm_year) + ".csv";
}
