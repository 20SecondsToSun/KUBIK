#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "cinder/gl/Texture.h"
#include "model/PhotoStorage.h"
#include "states/templateButtons/TemplateButtons.h"

namespace kubik
{
	namespace games
	{
		typedef	shared_ptr<class PhotoTemplate> PhotoTemplateRef;

		class PhotoTemplate: public IPhotoboothLocation
		{
			PhotoStorageRef photoStorage;
			ci::gl::Texture tex1, tex2;
			ci::Vec2f title1Pos, title2Pos;
			std::vector<ci::gl::Texture> templates, stickers;
			std::vector<TemplateButtonRef> templatebtns;

			void photoTemplateChoose(EventGUIRef& event);
			void setChoosingTemplate();
			TemplateButtonRef selectedTemplate;

		public:
			PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

			void start();
			void stop();		
			void reset(PhotoboothSettingsRef settings);	
			void update();
			void draw();
		};
	}
}