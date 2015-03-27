#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "ImageQuadroButton.h"
#include "SettingsFactory.h"
#include "SixButtonsLayer.h"
#include "OpenPhotoBoothLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class SixButtonsLayer<ChangeDesignEvent>> SixButtonsLayerDesignRef;
		typedef std::shared_ptr<class DesignChooser> DesignChooserRef;

		class DesignChooser : public Sprite
		{
		public:		
			DesignChooser(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			void buttonClicked(EventGUIRef& event);			

		private:
			ConfigSettingsRef configSettings;
			SixButtonsLayerDesignRef sixBtnLayer;
			int id;
		};
	}
}