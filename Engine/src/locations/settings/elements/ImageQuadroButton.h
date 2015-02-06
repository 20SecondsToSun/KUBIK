#pragma once

#include "TextTools.h"
#include "gui/SimpleButton.h"
#include "DesignData.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ImageQuadroButton> ImageQuadroButtonRef;

		class ImageQuadroButton: public SimpleButton
		{
		public:
			ImageQuadroButton(OneDesignItem item, ci::Vec2f pos)
				:SimpleButton(item.getIcon().getSize(), pos), item(item), selection(false)
			{
				textTex = textTools().getTextField(item.getTextItem());				
			}

			virtual void drawLayout()
			{
				gl::Texture icon = item.getIcon();

				gl::draw(icon);			
				
				if(selection)
				{
					gl::color(Color::hex(0xffff00));
					gl::lineWidth(7);
					gl::drawStrokedRoundedRect(ci::Rectf(ci::Vec2f::zero(), icon.getSize()), 6);
					gl::lineWidth(1);					
				}

				gl::draw(textTex, Vec2f((icon.getWidth() - textTex.getWidth()) * 0.5, icon.getHeight() + 10));
				gl::color(Color::white());			
			}		

			void setAlpha(float  alpha)
			{
			}	

			void setSelection(bool value)
			{
				selection = value;
			}

			OneDesignItem getItem() const
			{
				return item;
			}

		private:
			OneDesignItem item;
			gl::Texture textTex;
			bool selection;
		};	
	}
}