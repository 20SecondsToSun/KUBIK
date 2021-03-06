#pragma once
#include "main/IGameLocation.h"
#include "PhotoboothSettings.h"
#include "states/choosing/PhotoContainer.h"
#include "states/choosing/FilterSmallButton.h"
#include "TextTools.h"
#include "TimerTools.h"
#include "FilterChangedEvent.h"
#include "gui/ImageButtonSprite.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class PhotoChoosing> PhotoChoosingRef;

			class PhotoChoosing : public IGameLocation
			{
				static const int PHOTOS_NUM = 5;
				static const int MAX_SELECT = 3;

				enum locationState
				{
					ANIM_HIDE,
					CHOOSING
				}
				state;

				ci::Vec2f photoPositions[PHOTOS_NUM];
				ci::Vec2f titlePos, choosefonPos;
				ci::gl::Texture titleFilter, choosefon;
				ci::Anim<ci::Vec2f> titleFilterPos, choosefonPosAnim;				
				ci::Anim<float> alphaAnim;

				ImageButtonSpriteRef okBtn, reShotBtn;
				shaders::imagefilters::BaseShaderRef shader;

				int canSelectCount;
				int nowSelectCount;
				int selectedNum;
				float photoFiltersStartY;

				PhotoStorageRef photoStorage;
				std::vector<ci::Surface> thumbs;
				std::vector<PhotoContainerRef> photoBtns;
				std::vector<FilterSmallButtonRef> filterBtns;
				PhotoContainerRef lastSelected;

				void initShowAnimationParams();
				void showAnimationComplete();
				void drawPhotoPreview();
				void drawPhotoFilters();
				void photoChoosed(EventGUIRef& event);
				void filterChanged(EventGUIRef& event);
				void backToReshot(EventGUIRef& event);
				void filterSelected(int id);
				void okBtnClicked(EventGUIRef& event);
				void setShaderForPreviews();

			public:
				static const int RESHOT_LOC = 1;

				PhotoChoosing(config::PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);
				virtual void reset(config::ISettingsRef settings) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void draw() override;
				virtual void update() override;
				virtual void stopAllTweens() override;

				void setTitle();
				void setPhotoButtonsDesign();
				void setOkButtonDesign();
				void setReshotButtonDesign();
				void setFiltersData();
				void setFiltersTitleDesign();
				void setAdditionaBackgroung();
			};
		}
	}
}
