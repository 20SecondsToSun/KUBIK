#pragma once
#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"
#include "ConfigSettings.h"
#include "PrinterControlsHideEvent.h"
#include "PrinterStatResetEvent.h"
#include "TextTools.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr< class PrinterControls> PrinterControlsRef;

		class PrinterControls: public Sprite
		{
		public:	
			PrinterControls(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void drawLayout();
			virtual void draw();

		private:
			ConfigSettingsRef configSettings;
			TextItem titleText1, titleText2, iErrorText, changedText;
			SimpleSpriteButtonRef closeBtn, closeBtnBig, okBtn;
			ci::gl::Texture closeIcon;
			ci::Rectf okButtonArea, closeButtonArea;
			ci::Vec2f okTextPos;			
		};	
	}
}