#pragma once
#include "TextItem.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	namespace config
	{
		template <class EnumID>
		class ConfigTexts
		{
		public:
			
			typedef std::map <std::pair<string, EnumID>, TextItem> DicType;

			void insert(const std::string& lang, EnumID id, const TextItem& item)
			{
				auto key = std::make_pair(lang, id);
				dic[key] = item;
			}

			TextItem get(const std::string& lang, EnumID id)
			{
				auto key = std::make_pair(lang, id);
				return dic[key];
			}

			DicType& getDic()
			{
				return dic;
			}

			void setDic(const DicType& dic)
			{
				this->dic = dic;
			}

		private:
			DicType dic;	
		};
	}
}