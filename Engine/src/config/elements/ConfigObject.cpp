#include "ConfigObject.h"

using namespace kubik::config;
using namespace std;

void ConfigObject::setPathsConfigPath(const string& path)
{
	pathsPath = path;
}

void ConfigObject::setParamsConfigPath(const string& path)
{
	paramsPath = path;
}

void ConfigObject::setLabelsConfigPath(const string& path)
{
	labelsPath = path;
}

void ConfigObject::setConstsConfigPath(const string& path)
{
	constsPath = path;
}

const string& ConfigObject::getPathsConfigPath()
{
	return pathsPath;
}

const string& ConfigObject::getParamsConfigPath()
{
	return paramsPath;
}

const string& ConfigObject::getLabelsConfigPath()
{
	return labelsPath;
}

const string& ConfigObject::getConstsConfigPath()
{
	return constsPath;
}