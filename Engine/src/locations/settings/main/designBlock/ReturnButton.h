#pragma once

#include "TextTools.h"
#include "gui/SimpleButton.h"
#include "CloseDesignLayoutEvent.h"

namespace kubik
{
	namespace config
	{
		class ReturnButton: public SimpleButton
		{
		public:	
			ReturnButton(ConfigSettingsRef configSettings, Vec2i position)
				:SimpleButton(Rectf(0.0f, 0.0f, 707.0f, 175.0f)),
				icon(configSettings->getTexture("circles")),				
				text(configSettings->getTextItem(ConfigTextID::BACK))							 
			{
				setPosition(position);
				event = CloseDesignLayoutEventRef(new CloseDesignLayoutEvent());

				textTexture = textTools().getTextField(text);
				float width  = textTexture.getWidth() + icon.getWidth() + 28;
				startX = 0.5 * (707 - width);
			}
			
			virtual void drawLayout()
			{
				gl::color(text.getColor());
				gl::pushMatrices();
					gl::translate(startX, 95);
					gl::draw(icon);	
					gl::translate(icon.getWidth() + 28, -10);
					gl::draw(textTexture);
				gl::popMatrices();
				gl::color(Color::white());
			}

			void setAlpha(float  alpha)
			{
				text.setColor(Utils::colorAlpha(text.getColor(), alpha));	
			}

			//void setIcon(Texture tex)
			//{
			//	icon = tex;
			//}

			//void setText(string text)
			//{
			//	this->text = text;
			//}
	
		private:			
			Texture icon, textTexture;
			TextItem text;	
			float startX;
		};

		typedef std::shared_ptr<ReturnButton> ReturnButtonRef;
	}
}