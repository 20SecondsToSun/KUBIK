#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoOverElements: public IPhotoboothItem
		{
		public:	
			PhotoOverElements(ConfigSettingsRef configSettings, PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(configSettings, phbSettings, index, Color::hex(0x1f95ed),
				phbSettings->getMainTitles().getPhotoOverElementsText(),
				phbSettings->getSubTitles().getPhotoOverElementsText())
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoOverElements> PhotoOverElementsRef;
	}
}