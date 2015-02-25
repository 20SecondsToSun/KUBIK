#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "ChangePhotoFilterPreviewActiveEvent.h"
#include "ConfigSettings.h"
#include "ImageQuadroButton.h"
#include "SettingsFactory.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoFilters> PhotoFiltersRef;

		class PhotoFilters: public IPhotoboothItem
		{
		public:	
			PhotoFilters(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index);
			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void buttonClicked(EventGUIRef& event);

		private:
			std::map<int, ImageQuadroButtonRef> btns;			
		};		
	}
}