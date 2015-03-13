#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "FilterButton.h"
#include "CameraAdapter.h"
#include "TextTools.h"
#include "Filter.h"
#include "model/PhotoStorage.h"

namespace kubik
{
	namespace games
	{
		typedef shared_ptr<class PhotoFilter> PhotoFilterRef;

		class PhotoFilter : public IPhotoboothLocation
		{			
			std::vector<FilterButtonRef> filterBtns;	
			ci::gl::Texture title;
			ci::Vec2f titlePos;
			std::vector<config::PhotoboothSettings::Filter> filters;
			PhotoStorageRef photoStorage;

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
			
		public:	
			PhotoFilter(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

			virtual void start();
			virtual void stop();			
			virtual void reset(PhotoboothSettingsRef set);
			virtual void update();
			virtual void draw();

			void photoFilterSelect(EventGUIRef& event);
			int  getCountFiltersOn();

			void createfilters2();
			void createfilters3();
			void createfilters4();
			void createfilters5();
			void createfilters6();	
		};		
	}
}