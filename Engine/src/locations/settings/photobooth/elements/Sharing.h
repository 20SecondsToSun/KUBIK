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

			OneSharingItem(PhotoboothSettingsRef settings, PhtTextID id)
				:Sprite(), text(settings->getTextItem(id)),
				font(settings->getFont("introLight30")),
				icon(settings->getIcon(id)),
				settings(settings),
				id(id)
			{
				IconPair icons(icon, icon);
				checker = CheckerSocialRef(new CheckerSocial(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(131.0f, 78.0f)), icons, id));				
				checker->setActive(settings->getSocialState(id));
				addChild(checker);
			}

			void activateListeners()
			{
				checker->connectEventHandler(&OneSharingItem::checkerClicked, this);
				Sprite::activateListeners();
			}

			void unActivateListeners()
			{
				checker->disconnectEventHandler();
				Sprite::unActivateListeners();
			}

			void checkerClicked(EventGUIRef event)
			{
				mouseUpSignal(event);
			}

			void resetChecker()
			{
				checker->setActive(settings->getSocialState(id));
			}

			virtual void drawLayout()
			{
				textTools().textFieldDraw(text, ci::Vec2f(159.0f, 18.0f));
			}

			void writeValue()
			{
				settings->setSocialState(id, checker->getValue());
			}

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
			Sharing(PhotoboothSettingsRef phbSettings, ci::Color color, int index)
				:IPhotoboothItem(phbSettings, PhtTextID::PUBLISHING, color, index)
			{				
				typedef Pair<PhtTextID, ci::Vec2f> PosPair;
				std::vector<PosPair> pairs;

				pairs.push_back(PosPair(PhtTextID::PRINTER,     ci::Vec2f(134.0f, 400.0f)));
				pairs.push_back(PosPair(PhtTextID::EMAIL,       ci::Vec2f(134.0f, 585.0f)));
				pairs.push_back(PosPair(PhtTextID::QRCODE,      ci::Vec2f(134.0f, 775.0f)));

				pairs.push_back(PosPair(PhtTextID::FACEBOOK,    ci::Vec2f(521.0f, 400.0f)));
				pairs.push_back(PosPair(PhtTextID::VKONTAKTE,   ci::Vec2f(521.0f, 585.0f)));
				pairs.push_back(PosPair(PhtTextID::TWITTER,     ci::Vec2f(521.0f, 775.0f)));

				for (auto item : pairs)
				{
					auto shareEl = OneSharingItemRef(new OneSharingItem(phbSettings, item.param1));
					shareEl->setPosition(item.param2);
					list.push_back(shareEl);
					addChild(shareEl);
				}
			}			

			virtual void onOpenResetParams()
			{
				for (auto item : list)
					item->resetChecker();
			}

			virtual void draw()
			{
				IPhotoboothItem::draw();
				gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.5f));
			}

			virtual void saveConfiguration()
			{
				for (auto item : list)
					item->writeValue();
			}

		private:
			std::list<OneSharingItemRef> list;
		};

		typedef std::shared_ptr<Sharing> SharingRef;
	}
}