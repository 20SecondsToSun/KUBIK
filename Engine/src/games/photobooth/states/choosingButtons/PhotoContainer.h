#pragma once
#include "gui/SimpleSpriteButton.h"
#include "PhotoChoosedEvent.h"

namespace kubik
{
	typedef std::shared_ptr<class PhotoContainer> PhotoContainerRef;

	class PhotoContainer: public SimpleSpriteButton
	{	
		ci::gl::Texture galka, ramka;		
		bool isSelected;

	public:
		PhotoContainer(int id, const ci::gl::Texture& tex1, const ci::gl::Texture& tex2, const ci::Vec2f& vec);
		void setSelected(bool value);
		bool selected();
		virtual void drawLayout();
	};
}