#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoPrintCount: public IPhotoboothItem
		{
		public:	
			PhotoPrintCount(PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(phbSettings, index)
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoPrintCount> PhotoPrintCountRef;
	}
}