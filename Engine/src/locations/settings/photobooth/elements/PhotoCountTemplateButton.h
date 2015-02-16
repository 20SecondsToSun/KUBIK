#pragma once

#include "TextTools.h"
#include "gui/SimpleSpriteButton.h"
#include "DesignData.h"
#include "PhotoTemplateChooseEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoCountTemplateButton> PhotoCountTemplateButtonRef;

		class PhotoCountTemplateButton: public SimpleSpriteButton
		{
		public:
			PhotoCountTemplateButton(ci::gl::Texture tex, TextItem item, ci::Font font, int count, ci::Vec2f pos)
				:SimpleSpriteButton(tex.getSize(), pos), item(item), selection(false), tex(tex), count(count)
			{
				textTex  = textTools().getTextField(item);				
				countTex = textTools().getTextField(to_string(count), &font, Color::white());	
				event = PhotoTemplateChooseEventRef(new PhotoTemplateChooseEvent(count));
			}

			virtual void drawLayout()
			{
				gl::draw(tex);
			
				if(selection)
				{
					gl::color(Color::hex(0xffff00));
					gl::lineWidth(10);
					gl::drawStrokedRoundedRect(ci::Rectf(ci::Vec2f::zero(), tex.getSize()), 6);
					gl::lineWidth(1);					
				}

				//gl::drawSolidRect(buttonArea);

				gl::color(Color::white());
				gl::draw(textTex, Vec2f((tex.getWidth()  - textTex.getWidth()) * 0.5,  230));
				gl::draw(countTex, Vec2f((tex.getWidth() - countTex.getWidth()) * 0.5, -21));
			}		

			void setAlpha(float  alpha)
			{
			}	

			void setSelection(bool value)
			{
				selection = value;
			}

			bool getSelection()
			{
				return selection;
			}

		private:
			TextItem item;
			gl::Texture textTex, tex, countTex;
			int count;
			bool selection;
		};	
	}
}