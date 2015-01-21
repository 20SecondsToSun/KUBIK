#pragma once
#include "photobooth/elements/IPhotoboothItem.h"
#include "PhotoboothSettings.h"
#include "CheckerSocial.h"

namespace kubik
{
	namespace config
	{
		class OneSharingItem: public IDispatcher
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
				checker = CheckerSocialRef(new CheckerSocial(icons));				
				checker->setActive(settings->getSocialState(id));
			}

			void activateListeners()
			{
				checker->addMouseUpListener(&OneSharingItem::checkerClicked, this);
			}

			void unActivateListeners()
			{				
				checker->removeMouseUpListener();				
			}

			void checkerClicked(shared_ptr<kubik::Event> event)
			{
				checker->swapActive();								
				//mouseUpSignal(event);
			}

			void resetChecker()
			{
				checker->setActive(settings->getSocialState(id));
			}

			virtual void draw()
			{
				textTools().textFieldDraw(text, &font, Color::white(), position + Vec2f(159, 18));
				checker->draw();			
			}

			virtual void setPosition(Vec2f position)
			{				
				checker->setButtonArea(Rectf(position, position + Vec2f(131, 78)));
				IDispatcher::setPosition(position);
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

				pairs.push_back(PosPair(SocialID::PRINTER,     position + Vec2f(300, 350)));
				pairs.push_back(PosPair(SocialID::EMAIL,       position + Vec2f(300, 488)));
				pairs.push_back(PosPair(SocialID::QRCODE,      position + Vec2f(300, 626)));

				pairs.push_back(PosPair(SocialID::FACEBOOK,    position + Vec2f(687, 350)));
				pairs.push_back(PosPair(SocialID::VKONTAKTE,   position + Vec2f(687, 488)));
				pairs.push_back(PosPair(SocialID::TWITTER,     position + Vec2f(687, 626)));

				for (auto item : pairs)
				{
					OneSharingItemRef shareEl = OneSharingItemRef(new OneSharingItem(phbSettings, item.param1));
					shareEl->setPosition(item.param2);
					items.push_back(shareEl);
				}
			}

			void activateListeners()
			{
				for (auto item : items)
				{
					item->activateListeners();
				}
				IPhotoboothItem::activateListeners();
			}

			void unActivateListeners()
			{
				for (auto item : items)
				{
					item->unActivateListeners();
					item->removeMouseUpListener();
				}
				IPhotoboothItem::unActivateListeners();
			}

			virtual void onOpenResetParams()
			{
				for (auto item : items)
					item->resetChecker();
			}

			virtual void drawContent()
			{
				//gl::pushMatrices();
				//gl::translate(position);
				for (auto item : items)				
					item->draw();
				//gl::popMatrices();
			}

			virtual void saveConfiguration()
			{
				for (auto item : items)
					item->writeValue();
			};

		protected:
			list<OneSharingItemRef> items;
		};

		typedef std::shared_ptr<Sharing> SharingRef;
	}
}