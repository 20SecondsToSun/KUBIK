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
				IconPair icons(icon, settings->getEmptyIcon());
				checker = CheckerSocialRef(new CheckerSocial(Rectf(0.0f, 0.0f, 131.0f, 78.0f), icons, id));				
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
			Font font;
			TextItem text;			
			Texture icon;
			PhtTextID id;
			CheckerSocialRef checker;
			PhotoboothSettingsRef settings;			
		};

		typedef std::shared_ptr<OneSharingItem> OneSharingItemRef;

		class Sharing: public IPhotoboothItem
		{
		public:	
			Sharing(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, PhtTextID::PUBLISHING, color, index)
			{				
				typedef Pair<PhtTextID, Vec2f> PosPair;
				vector<PosPair> pairs;

				pairs.push_back(PosPair(PhtTextID::PRINTER,     Vec2f(134, 400)));
				pairs.push_back(PosPair(PhtTextID::EMAIL,       Vec2f(134, 585)));
				pairs.push_back(PosPair(PhtTextID::QRCODE,      Vec2f(134, 775)));

				pairs.push_back(PosPair(PhtTextID::FACEBOOK,    Vec2f(521, 400)));
				pairs.push_back(PosPair(PhtTextID::VKONTAKTE,   Vec2f(521, 585)));
				pairs.push_back(PosPair(PhtTextID::TWITTER,     Vec2f(521, 775)));

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
				gl::color(ColorA(1,1,1,0.5));
				//gl::draw(settings->getTexture("_shareTemp"), Vec2f(166, 0));
			}

			virtual void saveConfiguration()
			{
				for (auto item : list)
					item->writeValue();
			};

		protected:
			list<OneSharingItemRef> list;
		};

		typedef std::shared_ptr<Sharing> SharingRef;
	}
}