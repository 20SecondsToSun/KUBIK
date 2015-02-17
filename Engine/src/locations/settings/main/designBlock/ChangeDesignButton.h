#pragma once

#include "ImageQuadroButton.h"
#include "DesignData.h"
#include "ChangeDesignEvent.h"
#include "ChangePhotoOverDesignEvent.h"
#include "ChangePhotoCardStyleDesignEvent.h"
#include "ChangePhotoFilterPreviewActiveEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ChangeDesignButton> ChangeDesignButtonRef;
		typedef std::shared_ptr<class PhotoOverButton> PhotoOverButtonRef;
		typedef std::shared_ptr<class PhotoCardStyleButton> PhotoCardStyleButtonRef;
		typedef std::shared_ptr<class PhotoFilterPreviewButton> PhotoFilterPreviewButtonRef;
		
		class ChangeDesignButton: public ImageQuadroButton
		{
		public:
			ChangeDesignButton(OneDesignItem item, ci::Vec2f pos)
				:ImageQuadroButton(item, pos)
			{
				event = ChangeDesignEventRef(new ChangeDesignEvent(item));
			}		

		};	

		class PhotoOverButton: public ImageQuadroButton
		{
		public:
			PhotoOverButton(OneDesignItem item, ci::Vec2f pos)
				:ImageQuadroButton(item, pos)
			{
				event = ChangePhotoOverDesignEventRef(new ChangePhotoOverDesignEvent(item));
			}		

		};	

		class PhotoCardStyleButton: public ImageQuadroButton
		{
		public:
			PhotoCardStyleButton(OneDesignItem item, ci::Vec2f pos)
				:ImageQuadroButton(item, pos)
			{
				event = ChangePhotoCardStyleDesignEventRef(new ChangePhotoCardStyleDesignEvent(item));
			}	
		};	

		class PhotoFilterPreviewButton: public ImageQuadroButton
		{
		public:
			PhotoFilterPreviewButton(OneDesignItem item, ci::Vec2f pos)
				:ImageQuadroButton(item, pos)
			{
				event = ChangePhotoFilterPreviewActiveEventRef(new ChangePhotoFilterPreviewActiveEvent(item));
			}	
		};	

		
		
	}
}