#pragma once
#include "main/IGameLocation.h"
#include "PhotoboothSettings.h"
#include "FilterButton.h"
#include "CameraAdapter.h"
#include "TextTools.h"
#include "TimerTools.h"
#include "Filter.h"
#include "main/PhotoStorage.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class PhotoFilter> PhotoFilterRef;

			class PhotoFilter : public IGameLocation
			{
				enum sizeID
				{
					filter_2,
					filter_3_1,
					filter_3_2,
					filter_4,
					filter_5_1,
					filter_5_2,
					filter_6
				};

				struct TexParams
				{
					float sizex;
					float sizey;
					float offsetx;
					float offsety;
					float scale;
				};

				struct TexInfo
				{
					TexParams params;
					ci::gl::Texture texture;
				};

				enum locationState
				{
					TITLE_ANIM,
					UPDATE
				}state;

				std::vector<FilterButtonRef> filterBtns;
				ci::Vec2f titlePos;
				std::vector<config::PhotoboothSettings::Filter> filters;
				PhotoStorageRef photoStorage;

				std::map<sizeID, TexInfo> updateTextures;

				void createfilters2();
				void createfilters3();
				void createfilters4();
				void createfilters5();
				void createfilters6();

				void (PhotoFilter::* filterUpdate)();
				void filterUpdate2();
				void filterUpdate3();
				void filterUpdate4();
				void filterUpdate5();
				void filterUpdate6();

				void showAnimationComplete();
				void titleAnimComplete();

			public:
				PhotoFilter(config::PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void reset(config::ISettingsRef set) override;
				virtual void stopAllTweens() override;

				void photoFilterSelect(EventGUIRef& event);
				int  getCountFiltersOn();
				void skipLocation();
				void initStartAnimation();
				ci::gl::Texture getCanonCameraTexture(const TexParams& params);
				void setImageSizeParams();
			};
		}
	}
}