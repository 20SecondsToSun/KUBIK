#pragma once

namespace kubik
{
	namespace config
	{
		class ConfigObject
		{
		public:
			void setPathsConfigPath(const std::string& path);
			void setParamsConfigPath(const std::string& path);
			void setLabelsConfigPath(const std::string& path);
			void setConstsConfigPath(const std::string& path);

			const std::string& getPathsConfigPath();
			const std::string& getParamsConfigPath();
			const std::string& getLabelsConfigPath();
			const std::string& getConstsConfigPath();

		private:
			std::string pathsPath;
			std::string paramsPath;
			std::string labelsPath;
			std::string constsPath;
		};
	}
}