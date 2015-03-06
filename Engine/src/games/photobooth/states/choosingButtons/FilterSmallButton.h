#pragma once
#include "gui/SimpleSpriteButton.h"
#include "FilterChangedEvent.h"

namespace kubik
{
	typedef std::shared_ptr<class FilterSmallButton> FilterSmallButtonRef;

	class FilterSmallButton: public SimpleSpriteButton
	{	
		//ci::gl::Texture galka, ramka;		
		bool isSelected;
		int id;

		enum stateID
		{
			OPEN,
			CLOSE
		}state;

	public:
		FilterSmallButton(const ci::Vec2f& vec, int id);
		void setSelected(bool value);
		bool selected();
		virtual void drawLayout();
		int getID() const;

		void setOpenState();
		void setCloseState();
	};
}