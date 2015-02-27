#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "FilterButton.h"
#include "CameraAdapter.h"
#include "TextTools.h"

namespace kubik
{
	namespace games
	{
		typedef shared_ptr<class PhotoFilter> PhotoFilterRef;

		class PhotoFilter : public IPhotoboothLocation
		{			
			std::vector<FilterButtonRef> filterBtns;	
			ci::gl::Texture tex1, tex2;
			ci::Vec2f title1Pos, title2Pos;
			std::vector<int> filters;

		public:	
			PhotoFilter(PhotoboothSettingsRef settings);

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