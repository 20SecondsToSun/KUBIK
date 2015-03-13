#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"
#include "states/choosingButtons/PhotoContainer.h"
#include "states/choosingButtons/FilterSmallButton.h"
#include "TextTools.h"
#include "FilterChangedEvent.h"
#include "gui/ImageButtonSprite.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		typedef	shared_ptr<class PhotoChoosing> PhotoChoosingRef;

		class PhotoChoosing: public IPhotoboothLocation
		{
			static const int PHOTOS_NUM = 5;
			static const int MAX_SELECT = 3;

			ci::gl::Texture title, titleFilter, choosefon;
			ci::Anim<ci::Vec2f> titlePos, titleFilterPos, choosefonPos;
		
			ImageButtonSpriteRef okBtn, reShotBtn;
			shaders::imagefilters::BaseShaderRef shader;

			int canSelectCount;
			int nowSelectCount;
			int selectedNum;
			float photoFiltersStartY;	
			bool drawLocation;

			PhotoStorageRef photoStorage;
			std::vector<Surface> thumbs;
			std::vector<PhotoContainerRef> photoBtns;
			std::vector<FilterSmallButtonRef> filterBtns;			
			PhotoContainerRef lastSelected;

			void hidePreloaderComplete();
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

			PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

			void start();
			void stop();
			void reset(PhotoboothSettingsRef settings);		
			void draw();			
			void update();	
		};	
	}
}