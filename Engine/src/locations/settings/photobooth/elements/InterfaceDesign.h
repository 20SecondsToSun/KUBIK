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
				:IPhotoboothItem(configSettings,
									phbSettings, 
									index, 
									Color::hex(0x01a7fb),
									phbSettings->getMainTitles().getDesignInterfaceText(),
									phbSettings->getSubTitles().getDesignInterfaceText())
			{
				
				this->configSettings = configSettings;
				yourDizTex = textTools().getTextField(phbSettings->getYourDesignText(), &configSettings->getFont("introLight44"), Color::hex(0xffffff));
			}	

			virtual void draw()
			{
				IPhotoboothItem::draw();
				gl::color(ColorA(1,1,1,0.4));
				gl::draw(configSettings->getTexture("temp3"), position);
				gl::draw(yourDizTex, position + Vec2f(0.5*(itemWidth - yourDizTex.getWidth()), 540));
			}

		private:
			ConfigSettingsRef configSettings;
			Texture yourDizTex;
		};

		typedef std::shared_ptr<InterfaceDesign> InterfaceDesignRef;
	}
}