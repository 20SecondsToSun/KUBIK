#pragma once
#include "ConfigSettings.h"
#include "LoadButton.h"
#include "ImageQuadroButton.h"
#include "ChangeDesignButton.h"
#include "main/DesignBlock/ScreenSaverChecker.h"
#include "DecorLoadButton.h"
#include "PhotoboothSettings.h"

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

			LoadButtonRef createLoadButton(const std::string &path, const ci::Vec2f& pos)
			{
				return LoadButtonRef(
					new LoadButton(path, ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")));
			}

			LoadButtonRef createDecorLoadButton(const std::string &path, const ci::Vec2f& pos, const ci::gl::Texture& over)
			{
				return DecorLoadButtonRef(
					new DecorLoadButton(path,
					ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),					
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon"), over));
			}

			ScreenSaverCheckerRef createScreenSaverChecker(const ci::Vec2f& pos)
			{
				IconPair icons(settings->getTexture("ssCheckerOn"), settings->getTexture("ssCheckerOff"));
				ScreenSaverCheckerRef ch = ScreenSaverCheckerRef(new ScreenSaverChecker(Rectf(pos, pos + Vec2f(135.0f, 83.0f)), icons));
				ch->setActive(true);
				return ch;
			}

			ImageQuadroButtonRef createImageQuadroButton(OneDesignItem item, const ci::gl::Texture& texture, const ci::Vec2f& pos)
			{				
				ImageQuadroButtonRef iq = ImageQuadroButtonRef(new ImageQuadroButton(item, texture, pos));	
				return iq;				
			}

			ChangeDesignButtonRef createChangeDesignButton(OneDesignItem item, const ci::gl::Texture& texture, const ci::Vec2f& pos)
			{				
				return ChangeDesignButtonRef(new ChangeDesignButton(item, texture, pos));
			}		

			ImageQuadroButtonRef createPhotoOverButton(OneDesignItem item, const ci::gl::Texture& texture, const ci::Vec2f& pos)
			{				
				return PhotoOverButtonRef(new PhotoOverButton(item, texture, pos));
			}

			ImageQuadroButtonRef createCardStyleButton(OneDesignItem item, const ci::gl::Texture& texture, const ci::Vec2f& pos)
			{				
				return PhotoCardStyleButtonRef(new PhotoCardStyleButton(item, texture, pos));
			}			

			PhotoFilterPreviewButtonRef createPhotoFilterPreviewButton(OneDesignItem item, const ci::gl::Texture& texture, const ci::Vec2f& pos)
			{
				PhotoFilterPreviewButtonRef iq = PhotoFilterPreviewButtonRef(new PhotoFilterPreviewButton(item, texture, pos));
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
	
		inline SettingsFactory&	settingsFactory() { return SettingsFactory::getInstance();};
	}
}