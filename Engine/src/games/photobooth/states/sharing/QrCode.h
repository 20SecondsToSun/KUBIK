#pragma once

#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"
#include "DrawTools.h"
#include "model/PhotoStorage.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef shared_ptr<class QrCode> QrCodeRef;
			typedef shared_ptr<class QrCodeNull> QrCodeNullRef;
			
			class QrCode :public SimpleSpriteButton
			{
				ci::Vec2f position;
				ci::gl::Texture title;

			public:
				QrCode() :SimpleSpriteButton(Rectf(0.0, 0.0f, 424.0f, 424.0f))
				{

				};

				virtual void drawLayout()
				{
					gl::color(ColorA(color.r, color.g, color.b, alpha));	
					gl::pushMatrices();
					gl::translate(0.0f, 60.0f);
					gl::drawSolidRect(buttonArea);
					gl::popMatrices();					
					gl::draw(title, position);
					gl::color(Color::white());
					Sprite::drawLayout();
				}

				virtual void initLink(const std::string& link = "")
				{

				}

				virtual void setTtile(const ci::gl::Texture& texture)
				{
					title = texture;				
				}

			/*	virtual void setPosition(const ci::Vec2f& position)
				{
					SimpleSpriteButton::setPosition(position)
					this->position = position;
				}*/
				
			};

			class QrCodeNull :public QrCode
			{
				virtual void drawLayout()
				{

				}
			};
		}
	}
}