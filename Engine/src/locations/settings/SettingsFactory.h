#pragma once

#include "ConfigSettings.h"
#include "LoadButton.h"
#include "ImageQuadroButton.h"
#include "ChangeDesignButton.h"
#include "main/DesignBlock/ScreenSaverChecker.h"
#include "DecorLoadButton.h"
#include "PhotoboothSettings.h"
#include "PreloaderSettings.h"
#include "ScreenSaverSettings.h"

namespace kubik
{
	namespace config
	{
		class SettingsFactory
		{
		public:
			static SettingsFactory& getInstance()
			{ 
				static SettingsFactory tt; 
				return tt; 
			}

			LoadButtonRef createLoadButton(const std::string &path, const ci::Vec2f& pos)
			{
				return LoadButtonRef(
					new LoadButton(path, ci::Rectf(pos, pos + ci::Vec2f(200.0f, 70.0f)),
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")));
			}

			LoadButtonRef createDecorLoadButton(const std::string &path, const ci::Vec2f& pos, const ci::gl::Texture& over)
			{
				return DecorLoadButtonRef(new DecorLoadButton(path,	ci::Rectf(pos, pos + ci::Vec2f(200.0f, 70.0f)),					
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon"), over));
			}

			ScreenSaverCheckerRef createScreenSaverChecker(const ci::Vec2f& pos)
			{
				IconPair icons(settings->getTexture("ssCheckerOn"), settings->getTexture("ssCheckerOff"));
				ScreenSaverCheckerRef ch = ScreenSaverCheckerRef(new ScreenSaverChecker(ci::Rectf(pos, pos + ci::Vec2f(135.0f, 83.0f)), icons));
				ch->setActive(true);
				return ch;
			}

			ImageQuadroButtonRef createImageQuadroButton(const SixButtonOneData& data)
			{				
				return ImageQuadroButtonRef(new ImageQuadroButton(data));							
			}

			ChangeDesignButtonRef createChangeDesignButton(const SixButtonOneData& data)
			{
				ChangeDesignButtonRef value = ChangeDesignButtonRef(new ChangeDesignButton(data));
				return value;
			}

			ImageQuadroButtonRef createPhotoOverButton(const SixButtonOneData& data)
			{				
				PhotoOverButtonRef value = PhotoOverButtonRef(new PhotoOverButton(data));
				return value;
			}

			ImageQuadroButtonRef createCardStyleButton(const SixButtonOneData& data)
			{				
				PhotoCardStyleButtonRef value = PhotoCardStyleButtonRef(new PhotoCardStyleButton(data));
				return value;
			}			

			PhotoFilterPreviewButtonRef createPhotoFilterPreviewButton(const SixButtonOneData& data)
			{
				PhotoFilterPreviewButtonRef value = PhotoFilterPreviewButtonRef(new PhotoFilterPreviewButton(data));
				return value;
			}

			void inject(ConfigSettingsRef configSettings)
			{
				settings = configSettings;
			}

			void inject(PhotoboothSettingsRef configSettings)
			{
				phtSettings = configSettings;
			}

			void inject(PreloaderSettingsRef configSettings)
			{
				preloaderSettings = configSettings;
			}

			void inject(ScreenSaverSettingsRef configSettings)
			{
				screenSaverSettings = configSettings;
			}	

			ScreenSaverSettingsRef getScreenSaver()
			{
				return screenSaverSettings;
			}

			IMovieRef getMainPreloader() const
			{
				if (preloaderSettings)
					return preloaderSettings->getMainPreloader();

				return nullptr;
			}

			IMovieRef getMiniPreloader() const
			{
				if (preloaderSettings)
					return preloaderSettings->getMiniPreloader();

				return nullptr;
			}

		private:
			ConfigSettingsRef settings;
			PhotoboothSettingsRef phtSettings;
			PreloaderSettingsRef preloaderSettings;
			ScreenSaverSettingsRef screenSaverSettings;
		};
	
		inline SettingsFactory&	settingsFactory() { return SettingsFactory::getInstance();};
	}
}