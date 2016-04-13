#pragma once

#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class StatCollector> StatCollectorRef;

		class StatCollector
		{
			ConfigSettingsRef configSettings;

		public:
			static const std::string DELIMETER;

			StatCollector(ConfigSettingsRef configSettings);

			virtual void addPlayedGame(int count = 1);
			virtual void addPrintedCount();

			void addFBShare();
			void addVKShare();
			void addTWShare();
			void addEmailShare();

			std::string getDatePrefix() const;
			std::string getDBExt() const;
			std::string getTimeFormat() const;

		};
	}
}