#pragma once

namespace kubik
{
	namespace config
	{
		class ConfigObject
		{
		public:
			void setPathsConfigPath(const string& path)
			{
				pathsPath = path;
			}

			void setParamsConfigPath(const string& path)
			{
				paramsPath = path;
			}

			void setLabelsConfigPath(const string& path)
			{
				labelsPath = path;
			}

			void setConstsConfigPath(const string& path)
			{
				constsPath = path;
			}

			const string& getPathsConfigPath()
			{
				return pathsPath;
			}

			const string& getParamsConfigPath()
			{
				return paramsPath;
			}

			const string& getLabelsConfigPath()
			{
				return labelsPath;
			}

			const string& getConstsConfigPath()
			{
				return constsPath;
			}

		private:
			std::string pathsPath;
			std::string paramsPath;
			std::string labelsPath;
			std::string constsPath;
		};
	}
}