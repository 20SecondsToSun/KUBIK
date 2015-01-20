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
				id(id)
			{
				IconPair icons(icon, settings->getEmptyIcon());
				checker = CheckerSocialRef(new CheckerSocial(icons));				
				checker->addMouseUpListener(&OneSharingItem::checkerClicked, this);
				checker->setActive(settings->getSocialState(id));
			}

			void checkerClicked(shared_ptr<kubik::Event> event)
			{
				checker->swapActive();								
				//mouseUpSignal(event);
			}

			void resetChecker()
			{
				checker->setActive(id);
			}

			virtual void draw()
			{
				textTools().textFieldDraw(text, &font, Color::hex(0xffffff), position + Vec2f(159, 18));
				checker->draw();			
			}

			virtual void setPosition(Vec2f position)
			{
				checker->setButtonArea(Rectf(position.x, position.y, position.x + 131, position.y + 78));
				IDispatcher::setPosition(position);
			}

		private:
			Font font;
			string text;
			CheckerSocialRef checker;
			Texture icon;
			SocialID id;
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
					OneSharingItemRef shareEl = OneSharingItemRef(new OneSharingItem(settings, item.param1));
					shareEl->setPosition(item.param2);
					items.push_back(shareEl);
				}
			}

			virtual void draw()
			{				
				//gl::color(ColorA(1,1,1,0.4));
				//gl::draw(settings->getTexture("_temp"), position);
				//gl::color(Color::white());
				IPhotoboothItem::draw();
			}

			virtual void onOpenResetParams()
			{
				for (auto item : items)
					item->resetChecker();
			}

			virtual void drawContent()
			{
				for (auto item : items)				
					item->draw();
			}

		protected:
			list<OneSharingItemRef> items;

		};

		typedef std::shared_ptr<Sharing> SharingRef;
	}
}