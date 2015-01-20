#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoOverElements: public IPhotoboothItem
		{
		public:	
			PhotoOverElements(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, SettingsPartID::PHOTO_OVER, color, index)
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoOverElements> PhotoOverElementsRef;
	}
}