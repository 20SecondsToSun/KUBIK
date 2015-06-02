#pragma once

#include "HashTagBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

HashTagBlock::HashTagBlock(InstakubSettingsRef settings, const ci::Vec2i& position)
	:Sprite(),
	backgroundColor(Color::hex(0x9b881f)),
	titleTextPos(Vec2f(0.0f, 70.0f)),
	subTitleTextPos(Vec2f(0.0f, 145.0f))
{
	setPosition(position);

	titleItem = settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_MAIN);
	subTitleItem = settings->getTextItem(InstakubSettings::InstaTextID::HASHTAG_TITLE_SUB);

	searchfield = settings->getTexture("searchfield");
	searchfieldPos = Vec2f(0.5f * (910.0f - searchfield.getWidth()), 260.0f);

	hashTex = textTools().getTextField("#", &settings->getFont("introBook36"), ci::Color::hex(0x979797));
	hashPos = searchfieldPos + Vec2f(31.0f, 25.0f);
}

void HashTagBlock::drawLayout()
{
	gl::color(backgroundColor);
	gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(914.0f, 485.0f)));
	gl::color(Color::white());

	textTools().drawTextBox(titleItem, backgroundColor, titleTextPos, Vec2i(914.0f, 100.0f));
	textTools().drawTextBox(subTitleItem, backgroundColor, subTitleTextPos, Vec2i(914.0f, 50.0f));

	gl::draw(searchfield, searchfieldPos);
	gl::draw(hashTex, hashPos);
}