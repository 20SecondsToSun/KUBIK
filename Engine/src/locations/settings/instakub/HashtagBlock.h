#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "InstakubSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class HashTagBlock> HashTagBlockRef;

		class HashTagBlock: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;

			HashTagBlock(InstakubSettingsRef settings, Vec2i position):Sprite()
			{
				setPosition(position);	

				titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_MAIN));
				titleTextPos = ci::Vec2f(0.5 * (914 - titleTextTex.getWidth()), 127.0f - 77.0f + 52.0f);			

				subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_SUB));
				subTitleTextPos = ci::Vec2f(0.5 * (914 - subTitleTextTex.getWidth()), 179.0f - 13.0f);

				searchfield = settings->getTexture("searchfield");
				searchfieldPos = Vec2f(0.5*(910.0f - searchfield.getWidth()), 260.0f);
			
				hashTex = textTools().getTextField("#", &settings->getFont("introBook36"), ci::Color::hex(0x979797));
				hashPos = searchfieldPos + Vec2f(31.0f, 25.0f);	

				//addChild(touchKeyboard().getInstance());
			}				

			virtual void drawLayout()
			{
				gl::color(Color::hex(0x9b881f));
				gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(914.0f, 485.0f)));
				gl::color(Color::white());

				gl::draw(titleTextTex, titleTextPos);
				gl::draw(subTitleTextTex, subTitleTextPos);	

				gl::draw(searchfield, searchfieldPos);

				gl::draw(hashTex, hashPos);			
			}

			InstakubSettingsRef settings;	
			gl::Texture titleTextTex, subTitleTextTex, hashTex;	
			ci::Vec2f titleTextPos, subTitleTextPos, searchfieldPos, hashPos;
			ci::gl::Texture searchfield;
		};
	}
}