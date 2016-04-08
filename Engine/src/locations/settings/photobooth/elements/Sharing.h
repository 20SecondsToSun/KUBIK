#pragma once

#include "photobooth/elements/IPhotoboothItem.h"
#include "PhotoboothSettings.h"
#include "CheckerSocial.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace config
	{
		class OneSharingItem: public Sprite
		{
		public:
			typedef PhotoboothSettings::PhtTextID PhtTextID;

			OneSharingItem(PhotoboothSettingsRef settings, PhtTextID id);
			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void drawLayout();

			void checkerClicked(EventGUIRef event);
			void resetChecker();	

		private:
			ci::Font font;
			TextItem text;			
			ci::gl::Texture icon;
			PhtTextID id;
			CheckerSocialRef checker;
			PhotoboothSettingsRef settings;			
		};

		typedef std::shared_ptr<OneSharingItem> OneSharingItemRef;

		class Sharing: public IPhotoboothItem
		{
		public:	
			Sharing(PhotoboothSettingsRef phbSettings, ci::Color color, int index);

			virtual void onOpenResetParams();
			virtual void draw();			

		private:
			std::list<OneSharingItemRef> list;
		};

		typedef std::shared_ptr<Sharing> SharingRef;
	}
}