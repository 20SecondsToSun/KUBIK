#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"
#include "states/choosingButtons/PhotoContainer.h"
#include "states/choosingButtons/FilterSmallButton.h"
#include "TextTools.h"
#include "FilterChangedEvent.h"
#include "gui/ImageButtonSprite.h"

namespace kubik
{
	namespace games
	{
		typedef	shared_ptr<class PhotoChoosing> PhotoChoosingRef;

		class PhotoChoosing: public IPhotoboothLocation
		{
			static const int PHOTOS_NUM = 5;
			static const int MAX_SELECT = 3;

			ci::gl::Texture tex1, tex2, tex3, preloader;
			ci::Anim<ci::Vec2f> title1Pos, title2Pos, title3Pos, preloaderPos;
			ci::Anim<float> preloaderAlpha;
			ImageButtonSpriteRef okBtn;

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
			void filterSelected(int id);
			void okBtnClicked(EventGUIRef& event);

		public:
			PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

			void start();
			void stop();
			void reset(PhotoboothSettingsRef settings);		
			void draw();			
			void update();	
		};	
	}
}