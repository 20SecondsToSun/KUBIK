#include "DataBase.h"

using namespace kubik;
using namespace std;
using namespace ci;

DataBase& DataBase::getInstance()
{
	static DataBase base;
	return base;
}

void DataBase::saveData(const std::string& fullPath, const std::string& saveString)
{
	logger().log("save to................  " + fullPath);
	logger().log("save what&................  " + saveString);	


	std::ofstream logfile;
	logfile.open(fullPath.c_str(), ios::out | ios::app);
	logfile << saveString << std::endl;
	logfile.close();	
}

