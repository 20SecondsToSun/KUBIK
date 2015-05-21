#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "ChangePhotoCardStyleDesignEvent.h"
#include "SettingsFactory.h"
#include "SixButtonsLayer.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoCardStyle> PhotoCardStyleRef;
		typedef std::shared_ptr<class SixButtonsLayer<ChangePhotoCardStyleDesignEvent>> SixButtonsLayerPhotoCardRef;

		class PhotoCardStyle: public IPhotoboothItem
		{
		public:	
			PhotoCardStyle(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index);

			void activateListeners() override;
			void unActivateListeners() override;
			void buttonClicked(EventGUIRef& event);

		private:
			SixButtonsLayerPhotoCardRef sixBtnLayer;
		};		
	}
}