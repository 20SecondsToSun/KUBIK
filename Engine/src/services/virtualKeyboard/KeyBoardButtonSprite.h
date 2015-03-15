#pragma once
#include "gui/SimpleSpriteButton.h"
#include "cinder/Timeline.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class KeyBoardButtonSprite: public SimpleSpriteButton
	{
	public:	

		KeyBoardButtonSprite(const ci::gl::Texture& image, ci::Font _font, const std::string& _label)
			:SimpleSpriteButton(image.getWidth(), image.getHeight(), Vec2f::zero()), image(image),
			overColor(Color::white()), code(_label), font(_font), isTextField(true)
		{
			createTextField();
			alpha = 1;
		}

		KeyBoardButtonSprite(ci::gl::Texture image, std::string _label)
			:SimpleSpriteButton(image.getWidth(), image.getHeight(), Vec2f::zero()), image(image),
			overColor(Color::white()), code(_label),  isTextField(false)
		{
			alpha = 1;
		}

		void drawLayout()
		{
			gl::color(Color::white());	
			gl::color(Utils::colorAlpha(overColor.value(), alpha));	
			gl::draw(image, Vec2f(0.0f, 3.0f));		
			gl::color(Color::white());	
			
			if(textTexture)
				gl::draw(textTexture,
					Vec2f(0.5f*(buttonArea.getWidth() - textTexture.getWidth()),
					0.5f*(buttonArea.getHeight() - textTexture.getHeight())));
			gl::color(Color::white());	
			//gl::drawSolidRect(buttonArea);
		}

		void changeTexture(const Texture& image)
		{
			this->image = image;
		}

		std::string getBtnId() const
		{
			return code;
		}

		void  setBtnId(const std::string& value)
		{
			if (isTextField) 
			{
				code = value;
				createTextField();	
			}
		}

		void  createTextField()
		{
			TextLayout simple;
			simple.setFont( font );	
			simple.setColor( Color::black());
			simple.addLine(Utils::cp1251_to_utf8(code.c_str()));		
			textTexture = gl::Texture( simple.render( true, false ) );	
		}	

		void down()
		{	
			timeline().apply( &overColor,  Color::hex(0x9dc9f6), Color::white() , 0.5f, EaseInBack()).finishFn( [ & ]()
			{
			});	
		}

		void setAlpha(float alpha)
		{
			this->alpha = alpha;
		}

	private:
		ci::gl::Texture image;	
		ci::Anim<ci::Color> overColor;
		gl::Texture textTexture;
		std::string code;
		ci::Font font;
		bool isTextField;
	//	float alpha;
	};

	typedef shared_ptr<KeyBoardButtonSprite> KeyBoardButtonSpriteRef;
}