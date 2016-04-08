#pragma once

namespace kubik
{
	namespace config
	{
		class Paths
		{

		public:
			static std::string getPopupFontPath()
			{
				return ci::app::getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf";
			}

			static ci::fs::path getPhotoDownloadedPath()
			{
				return ci::app::getAppPath() / "Kubik//photoDir";
			}

			static std::string getInterfacePath()
			{
				return "kubik\\interface\\";
			}

			static ci::fs::path getBasePath()
			{
				return ci::app::getAppPath();
			}

			static std::string getKeyboardPath()
			{
				return "kubik\\design_elements\\keyboard\\";
			}

			static ci::fs::path getFontsPath()
			{
				return ci::app::getAppPath() / "data\\fonts\\";
			}

			static ci::fs::path getAppConfigFile()
			{
				return ci::app::getAppPath() / "data/configs/app.txt";
			}

			static ci::fs::path getPhotoTemplatePath()
			{
				return ci::app::getAppPath() / "//template.png";
			}

			static std::string getPreloaderPath()
			{
				return (ci::app::getAppPath() / "kubik/design_elements/preloader/").string();
			}

			static ci::fs::path getLogPath(const std::string& logName)
			{
				return ci::app::getAppPath() / "logs" / logName;
			}

			static ci::fs::path getMemoryLogPath(const std::string& logName)
			{
				return ci::app::getAppPath() / "logs" / logName;
			}
		};
	}
}
