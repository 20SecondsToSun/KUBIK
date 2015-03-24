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
			ChangeDesignButton(const OneDesignItem& item, const ci::Vec2f& pos);

		};	

		class PhotoOverButton: public ImageQuadroButton
		{
		public:
			PhotoOverButton(const OneDesignItem& item, const ci::Vec2f& pos);

		};	

		class PhotoCardStyleButton: public ImageQuadroButton
		{
		public:
			PhotoCardStyleButton(const OneDesignItem& item, const ci::Vec2f& pos);			
		};	

		class PhotoFilterPreviewButton: public ImageQuadroButton
		{
		public:
			PhotoFilterPreviewButton(const OneDesignItem& item, const ci::Vec2f& pos);			
		};	

		template <class Type> class SixItemButton: public ImageQuadroButton
		{
		public:
			SixItemButton(const OneDesignItem& item, const ci::Vec2f& pos) :ImageQuadroButton(item, pos)
			{
				event = std::shared_ptr<Type>(new Type(item));
			}	
		};			
	}
}