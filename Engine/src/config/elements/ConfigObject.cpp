#include "ConfigObject.h"

using namespace kubik::config;

void ConfigObject::setPathsConfigPath(const std::string& path)
{
	pathsPath = path;
}

void ConfigObject::setParamsConfigPath(const std::string& path)
{
	paramsPath = path;
}

void ConfigObject::setLabelsConfigPath(const std::string& path)
{
	labelsPath = path;
}

void ConfigObject::setConstsConfigPath(const std::string& path)
{
	constsPath = path;
}

const std::string& ConfigObject::getPathsConfigPath()
{
	return pathsPath;
}

const std::string& ConfigObject::getParamsConfigPath()
{
	return paramsPath;
}

const std::string& ConfigObject::getLabelsConfigPath()
{
	return labelsPath;
}

const std::string& ConfigObject::getConstsConfigPath()
{
	return constsPath;
}