#pragma once
#include "main/IGameLocation.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "TimerTools.h"
#include "cinder/gl/Texture.h"
#include "main/PhotoStorage.h"
#include "states/template/TemplateButtons.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class PhotoTemplate> PhotoTemplateRef;

			class PhotoTemplate : public IGameLocation
			{
				enum locationState
				{
					INIT,
					ANIM_HIDE,
					TEMPLATE_CHOOSE
				}
				state;

				PhotoStorageRef photoStorage;

				std::vector<ci::gl::Texture> cards;
				std::vector<ci::gl::Texture>	stickers;

				std::vector<TemplateButtonRef> templatebtns;
				ci::Anim<float> alphaAnim;
				ci::Vec2f titlePos;

				void photoTemplateChoose(EventGUIRef& event);
				void setChoosingTemplate();
				void showAnimationComplete();
				void initShowAnim();
				void startHideAnimation();

				TemplateButtonRef selectedTemplate;

				std::string templateName = "//template.png";


				ci::gl::Texture tempImage;

			public:
				PhotoTemplate(config::PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

				virtual void reset(config::ISettingsRef settings) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;

				void resetTemplateButtons();
				virtual void stopAllTweens() override;
			};
		}
	}
}