#pragma once

#include "HashTagBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

HashTagBlock::HashTagBlock(InstakubSettingsRef settings, const ci::Vec2i& position):Sprite()
{
	setPosition(position);	

	titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_MAIN));
	titleTextPos = Vec2f(0.5f * (914.0f - titleTextTex.getWidth()), 100.0f);			

	subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_SUB));
	subTitleTextPos = Vec2f(0.5f * (914.0f - subTitleTextTex.getWidth()), 166.0f);

	searchfield = settings->getTexture("searchfield");
	searchfieldPos = Vec2f(0.5f * (910.0f - searchfield.getWidth()), 260.0f);

	hashTex = textTools().getTextField("#", &settings->getFont("introBook36"), ci::Color::hex(0x979797));
	hashPos = searchfieldPos + Vec2f(31.0f, 25.0f);	
}				

void HashTagBlock::drawLayout()
{
	gl::color(Color::hex(0x9b881f));
	gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(914.0f, 485.0f)));
	gl::color(Color::white());

	gl::draw(titleTextTex, titleTextPos);
	gl::draw(subTitleTextTex, subTitleTextPos);	

	gl::draw(searchfield, searchfieldPos);
	gl::draw(hashTex, hashPos);			
}
