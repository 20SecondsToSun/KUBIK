#pragma once
#include "photobooth/elements/IPhotoboothItem.h"

namespace kubik
{
	namespace config
	{
		class InterfaceDesign: public IPhotoboothItem
		{
		public:	
			InterfaceDesign(ConfigSettingsRef configSettings, PhotoboothSettingsRef phbSettings, int index)
				:IPhotoboothItem(phbSettings, index)
			{
				this->configSettings = configSettings;
				mainText = "Дизайн интерфейса";
				subText  = "Minimal Epileptic";
				yourDizText = "Ваш дизайн";
				saveText = "Сохранить";
			}	

			virtual void draw()
			{
				IPhotoboothItem::draw();
				gl::color(ColorA(1,1,1,0.4));
				gl::draw(configSettings->getTexture("temp3"), position);
			}

		private:
			ConfigSettingsRef configSettings;
			string yourDizText, saveText;
		};

		typedef std::shared_ptr<InterfaceDesign> InterfaceDesignRef;
	}
}