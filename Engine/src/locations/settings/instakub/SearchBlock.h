#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "HashChecker.h"

namespace kubik
{
	namespace config
	{
		class SearchBlock: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;

			SearchBlock(InstakubSettingsRef settings, Vec2i position):Sprite()
			{
				setPosition(position);	

				titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_MAIN));
				titleTextPos = ci::Vec2f(0.5 * (914 - titleTextTex.getWidth()), 127.0f - 77 + 52);			

				subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_SUB));
				subTitleTextPos = ci::Vec2f(0.5 * (914 - subTitleTextTex.getWidth()), 179.0f - 13);

				IconPair icons(settings->getTexture("checkerw"), settings->getTexture("checkerw"));
				checker = HashCheckerRef(new HashChecker(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(131.0f, 78.0f)), icons));				
				checker->setActive(settings->getSearchFlag());
				checker->setPosition(Vec2f(390, 243));
				addChild(checker);
			}	

			void activateListeners()
			{
				checker->connectEventHandler(&SearchBlock::checkerClicked, this);
			}

			void unActivateListeners()
			{
				checker->disconnectEventHandler();
			}

			void checkerClicked(EventGUIRef event)
			{
				mouseUpSignal(event);
			}

			void openDirectory(EventGUIRef event)
			{
				//if(eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER])
				//	eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER]();	
			}
			
			virtual void drawLayout()
			{
				gl::color(Color::hex(0xdc831a));
				gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(914, 435)));
				gl::color(Color::white());

				gl::draw(titleTextTex, titleTextPos);
				gl::draw(subTitleTextTex, subTitleTextPos);				
			}
	
			void setAlpha(float alpha)
			{
				//textcolor = Utils::colorAlpha(textcolor, alpha);	
			}

		private:
			InstakubSettingsRef settings;	
			gl::Texture titleTextTex, subTitleTextTex;	
			ci::Vec2f titleTextPos, subTitleTextPos;
			HashCheckerRef checker;
		};

		typedef std::shared_ptr<SearchBlock> SearchBlockRef;
	}
}