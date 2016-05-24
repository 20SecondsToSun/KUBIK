#pragma once
#include "gui/SimpleSpriteButton.h"
#include "PozaSettings.h"
#include "TimerTools.h"
#include "gui/ImageButtonSprite.h"
#include "main/IGameLocation.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			typedef std::shared_ptr<class Printer> PrinterRef;

			class Printer :public IGameLocation
			{
				ci::gl::Texture totalfail;
				ci::gl::Texture printerbg;
				ci::gl::Texture currentCardTemplate;
				

				ci::Vec2f titleTexPos;
				ci::Anim<float> alphaAnim;
				ImageButtonSpriteRef printBtn, againBtn, othergamesBtn;
				photobooth::PhotoStorageRef photoStorage;

				
				float animTime, buttonY;

				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();

				void printBtnHandler(EventGUIRef& event);
				void againBtnHandler(EventGUIRef& event);
				void othergamesBtnHandler(EventGUIRef& event);

				enum class PrinterState
				{
					FAIL,
					PASS
				}
				state;

				void disconnectEventHandlers();

				std::vector<photobooth::PhotoItem> photoItems;
				std::vector<int>& gameScore;
			public:
				Printer(config::PozaSettingsRef settings, photobooth::PhotoStorageRef photoStorage, std::vector<int>& gameScore);
				~Printer();
				virtual void reset(config::ISettingsRef set) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void stopAllTweens() override;
			};
		}
	}
}