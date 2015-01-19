#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoOverElements: public IPhotoboothItem
		{
		public:	
			PhotoOverElements(PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(phbSettings, index)
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoOverElements> PhotoOverElementsRef;
	}
}