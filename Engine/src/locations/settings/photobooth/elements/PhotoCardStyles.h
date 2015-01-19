#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoCardStyles: public IPhotoboothItem
		{
		public:	
			PhotoCardStyles(PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(phbSettings, index)
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoCardStyles> PhotoCardStylesRef;
	}
}