#pragma once
#include "ConfigSettings.h"
#include "LoadButton.h"
#include "ImageQuadroButton.h"
#include "ChangeDesignButton.h"
#include "main/DesignBlock/ScreenSaverChecker.h"
#include "DecorLoadButton.h"
#include "photobooth/elements/PhotoCountTemplateButton.h"

using namespace ci;

namespace kubik
{
	namespace config
	{
		class SettingsFactory
		{
		public:

			static SettingsFactory& getInstance() { 
				static SettingsFactory tt; 
				return tt; 
			};

			LoadButtonRef createLoadButton(ci::Vec2f pos)
			{
				return LoadButtonRef(
					new LoadButton(ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")					
					));
			}

			LoadButtonRef createDecorLoadButton(ci::Vec2f pos)
			{
				return DecorLoadButtonRef(
					new DecorLoadButton(ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")					
					));
			}

			ScreenSaverCheckerRef createScreenSaverChecker(ci::Vec2f pos)
			{
				IconPair icons(settings->getTexture("ssCheckerOn"), settings->getTexture("ssCheckerOff"));
				ScreenSaverCheckerRef ch = ScreenSaverCheckerRef(new ScreenSaverChecker(Rectf(pos, pos + Vec2f(135, 83)), icons));				
				ch->setActive(true);
				return ch;
			}

			ImageQuadroButtonRef createImageQuadroButton(OneDesignItem item, ci::Vec2f pos)
			{				
				ImageQuadroButtonRef iq = ImageQuadroButtonRef(new ImageQuadroButton(item, pos));	
				return iq;				
			}

			ChangeDesignButtonRef createChangeDesignButton(OneDesignItem item, ci::Vec2f pos)
			{				
				ChangeDesignButtonRef iq = ChangeDesignButtonRef(new ChangeDesignButton(item, pos));	
				return iq;				
			}

			PhotoCountTemplateButtonRef createPhotoCountTemplateButton(ci::gl::Texture tex, PhtTextID item, int count, ci::Vec2f pos = Vec2f::zero())
			{	
				PhotoCountTemplateButtonRef iq = PhotoCountTemplateButtonRef(new PhotoCountTemplateButton(tex, phtSettings->getTextItem(item), phtSettings->getFont("introb210"), count, pos));	
				return iq;				
			}

			void inject(ConfigSettingsRef configSettings)
			{
				settings = configSettings;
			}

			void inject(PhotoboothSettingsRef configSettings)
			{
				phtSettings = configSettings;
			}

		private:
			ConfigSettingsRef settings;
			PhotoboothSettingsRef phtSettings;

		};
		// helper function(s) for easier access 
		inline SettingsFactory&	settingsFactory() { return SettingsFactory::getInstance();};
	}
}

