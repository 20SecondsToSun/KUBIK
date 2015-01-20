#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoCardStyles: public IPhotoboothItem
		{
		public:	
			PhotoCardStyles(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, SettingsPartID::CARD_STYLE, color, index)
			{
			
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoCardStyles> PhotoCardStylesRef;
	}
}