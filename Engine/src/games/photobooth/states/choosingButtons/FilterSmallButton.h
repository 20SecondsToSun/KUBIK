#pragma once
#include "gui/SimpleSpriteButton.h"
#include "FilterChangedEvent.h"
#include "TextTools.h"

namespace kubik
{
	typedef std::shared_ptr<class FilterSmallButton> FilterSmallButtonRef;

	class FilterSmallButton: public SimpleSpriteButton
	{	
		//ci::gl::Texture galka, ramka;		
		bool isSelected;
		int id;
		std::string text;
		ci::Font fontC, fontO;

		gl::Texture titleSmall, titleBig;
		Vec2f titleSmallPos, titleBigPos;

		enum stateID
		{
			OPEN,
			CLOSE
		}state;

	public:
		FilterSmallButton(const ci::Vec2f& vec, int id, const std::string &text, ci::Font fontC, ci::Font fontO);
		void setSelected(bool value);
		bool selected();
		virtual void drawLayout();
		int getID() const;

		void setOpenState();
		void setCloseState();
	};
}