#include "model/PozaBase.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::poza;

void PozaBase::shuffleDataBase()
{
	data = Utils::shuffleVector(data);
	index = 0;
}

kubik::config::PozaSettings::PozaDataObj PozaBase::getPose()
{
	auto pose = data[index];
	if (++index >= data.size())
	{
		shuffleDataBase();
		index = 0;
	}
	return pose;
}