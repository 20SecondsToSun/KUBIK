#pragma once

namespace kubik
{
	class DataBase
	{
	public:
		static DataBase& getInstance();
		void saveData(const std::string& fullPath, const std::string& saveString);

	//private:
		
	};

	inline DataBase& data_base()
	{
		return DataBase::getInstance();
	};
}