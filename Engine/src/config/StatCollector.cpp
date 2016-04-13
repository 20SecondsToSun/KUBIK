#include "StatCollector.h"

using namespace kubik::config;

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
