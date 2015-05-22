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
			ChangeDesignButton(const SixButtonOneData& item);
		};	

		class PhotoOverButton: public ImageQuadroButton
		{
		public:
			PhotoOverButton(const SixButtonOneData& item);
		};	

		class PhotoCardStyleButton: public ImageQuadroButton
		{
		public:
			PhotoCardStyleButton(const SixButtonOneData& item);
		};	

		class PhotoFilterPreviewButton: public ImageQuadroButton
		{
		public:
			PhotoFilterPreviewButton(const SixButtonOneData& item);
		};	

		template <class Type> class SixItemButton: public ImageQuadroButton
		{
		public:
			SixItemButton(const SixButtonOneData& item):ImageQuadroButton(item)
			{
				event = std::shared_ptr<Type>(new Type(item.getDesignItem()));
			}	
		};			
	}
}