#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoCardStyles: public IPhotoboothItem
		{
		public:	
			PhotoCardStyles(ConfigSettingsRef configSettings, PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(configSettings,
				phbSettings, index, Color::hex(0x3e82df),
				phbSettings->getMainTitles().getPhotoStyleText(),
				phbSettings->getSubTitles().getPhotoStyleText())
			{
			
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoCardStyles> PhotoCardStylesRef;
	}
}