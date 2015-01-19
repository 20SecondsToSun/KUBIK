#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoPrintCount: public IPhotoboothItem
		{
		public:	
			PhotoPrintCount(ConfigSettingsRef configSettings, PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(configSettings, phbSettings, index, Color::hex(0x5e6fd1),
				phbSettings->getMainTitles().getPhotoPrintCountText(),
				phbSettings->getSubTitles().getPhotoPrintCountText())
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoPrintCount> PhotoPrintCountRef;
	}
}