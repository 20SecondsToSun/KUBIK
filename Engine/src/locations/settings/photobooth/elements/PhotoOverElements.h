#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "ChangePhotoOverDesignEvent.h"
#include "SettingsFactory.h"
#include "SixButtonsLayer.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoOverElements> PhotoOverElementsRef;
		typedef std::shared_ptr<class SixButtonsLayer<ChangePhotoOverDesignEvent>> SixButtonsLayerPhotoOverRef;

		class PhotoOverElements: public IPhotoboothItem
		{
		public:	
			PhotoOverElements(PhotoboothSettingsRef settings, const ci::Color& color, int index);
			virtual void activateListeners();
			virtual void unActivateListeners();
			void buttonClicked(EventGUIRef& event);

		private:
			SixButtonsLayerPhotoOverRef sixBtnLayer;
		};		
	}
}