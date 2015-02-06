#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoPrintCount: public IPhotoboothItem
		{
		public:	
			PhotoPrintCount(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, PhtTextID::PRINT_COUNT, color, index)
			{
				
			}	

		protected:

		};

		typedef std::shared_ptr<PhotoPrintCount> PhotoPrintCountRef;
	}
}