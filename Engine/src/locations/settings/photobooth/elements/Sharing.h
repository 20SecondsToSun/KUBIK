#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "PhotoboothSettings.h"
#include "CheckerSocial.h"

namespace kubik
{
	namespace config
	{
		class OneSharingItem: public CompositeDispatcher
		{
		public:
			typedef PhotoboothSettings::SocialID SocialID;

			OneSharingItem(PhotoboothSettingsRef settings, SocialID id)
				:text(settings->getSocialTitle(id)),
				font(settings->getFont("introLight30")),
				icon(settings->getIcon(id)),
				settings(settings),
				id(id)
			{
				IconPair icons(icon, settings->getEmptyIcon());
				checker = CheckerSocialRef(new CheckerSocial(Rectf(0.0f, 0.0f, 131.0f, 78.0f), icons));				
				checker->setActive(settings->getSocialState(id));
				addChild(checker);
			}

			void activateListeners()
			{
				checker->addMouseUpListener(&OneSharingItem::checkerClicked, this);
				CompositeDispatcher::activateListeners();
			}

			void checkerClicked(EventGUIRef event)
			{
				checker->swapActive();								
				mouseUpSignal(event);
			}

			void resetChecker()
			{
				checker->setActive(settings->getSocialState(id));
			}

			virtual void drawLayout()
			{
				textTools().textFieldDraw(text, &font, Color::white(), Vec2f(159.0f, 18.0f));
			}

			void writeValue()
			{
				settings->setSocialState(id, checker->getValue());
			}

		private:
			Font font;
			string text;			
			Texture icon;
			SocialID id;
			CheckerSocialRef checker;
			PhotoboothSettingsRef settings;			
		};

		typedef std::shared_ptr<OneSharingItem> OneSharingItemRef;

		class Sharing: public IPhotoboothItem
		{
		public:	
			typedef PhotoboothSettings::SocialID SocialID;

			Sharing(PhotoboothSettingsRef phbSettings, Color color, int index)
				:IPhotoboothItem(phbSettings, SettingsPartID::SHARING, color, index)
			{				
				typedef Pair<SocialID, Vec2f> PosPair;
				vector<PosPair> pairs;

				pairs.push_back(PosPair(SocialID::PRINTER,     Vec2f(134, 350)));
				pairs.push_back(PosPair(SocialID::EMAIL,       Vec2f(134, 488)));
				pairs.push_back(PosPair(SocialID::QRCODE,      Vec2f(134, 626)));

				pairs.push_back(PosPair(SocialID::FACEBOOK,    Vec2f(521, 350)));
				pairs.push_back(PosPair(SocialID::VKONTAKTE,   Vec2f(521, 488)));
				pairs.push_back(PosPair(SocialID::TWITTER,     Vec2f(521, 626)));

				for (auto item : pairs)
				{
					OneSharingItemRef shareEl = OneSharingItemRef(new OneSharingItem(phbSettings, item.param1));
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