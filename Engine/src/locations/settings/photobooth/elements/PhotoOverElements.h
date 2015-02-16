#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class PhotoOverElements: public IPhotoboothItem
		{
		public:	
			PhotoOverElements(PhotoboothSettingsRef settings, Color color, int index)
				:IPhotoboothItem(settings, PhtTextID::PHOTO_OVER, color, index)
			{
				DesignData designdata = settings->getPhotoOverDesignData();				
				
				Vec2f pos = Vec2f::zero();
				int i = 0;
				float shiftX = 53, shiftY = 130;

				for (auto it : designdata)
				{				 
					Texture icon = settings->getTexture(it.getName());
					it.setIcon(icon);
					it.setFont(settings->getFonts());

					pos.x = (icon.getWidth() + shiftX) * (i % 3);
					pos.y = (icon.getWidth() + shiftY) * (i / 3);

					ImageQuadroButtonRef imageQuadroButton = settingsFactory().createChangeDesignButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	

					i++;					
				}
			}	

		private:
			std::map<int, ImageQuadroButtonRef> btns;	

		};

		typedef std::shared_ptr<PhotoOverElements> PhotoOverElementsRef;
	}
}