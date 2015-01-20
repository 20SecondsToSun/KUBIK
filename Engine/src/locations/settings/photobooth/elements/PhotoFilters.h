#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		class PhotoFilters: public IPhotoboothItem
		{
		public:	
			PhotoFilters(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, SettingsPartID::FILTERS, color, index)
			{
				
			}	

			virtual void draw()
			{
				IPhotoboothItem::draw();
			}

		private:
			ConfigSettingsRef configSettings;
			Texture yourDizTex;
		};

		typedef std::shared_ptr<PhotoFilters> PhotoFiltersRef;
	}
}