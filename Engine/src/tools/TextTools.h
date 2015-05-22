#pragma once

#include "StringTools.h"
#include "TextItem.h"

using namespace ci;
namespace kubik
{
	class TextTools
	{
	public:

		static TextTools& getInstance() { 
			static TextTools tt; 
			return tt; 
		};

		void textFieldDraw(const std::string& text, ci::Font *font, const ci::ColorA& color, const Vec2f& coords = Vec2f::zero())
		{
			gl::pushMatrices();
			gl::translate(coords);
			ci::TextLayout simple;
			//simple.clear(ColorA(1,1,1,0));
			simple.setFont( *font );
			simple.setColor(color );
			simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));				
			gl::Texture render = gl::Texture( simple.render( true, false ) );
			gl::draw(render);
			gl::popMatrices();
			gl::color(Color::white());
		}

		gl::Texture  getTextField(const std::string& text, ci::Font* font, const ci::ColorA& color)
		{	
			ci::TextLayout simple;			
			simple.clear(ColorA(1,1,1,0));
			simple.setFont( *font );
			simple.setColor(color );
			simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));			
			return gl::Texture( simple.render( true, false ) );
		}

		gl::Texture  getTextField(const char* text, ci::Font* font, const ci::ColorA& color)
		{		
			ci::TextLayout simple;
			simple.clear(ColorA(1,1,1,0));
			simple.setFont( *font );
			simple.setColor(color );
			simple.addLine(stringTools().cp1251_to_utf8(text));			
			return gl::Texture( simple.render( true, false ) );
		}

		ci::Surface getTextField(const TextItem& item, bool isCentered = false, float offset = 0)
		{
			ci::TextLayout simple;
			simple.clear(ColorA(1,1,1,0));
			simple.setFont( item.getFont());
			simple.setColor(item.getColor());
			
			if(isCentered)
			{			
				auto vec = stringTools().split(item.getText(), '\n');
				simple.setLeadingOffset(offset);
				for (auto it : vec)				
					simple.addCenteredLine(stringTools().cp1251_to_utf8(it.c_str()));				
			}
			else
				simple.addLine(stringTools().cp1251_to_utf8(item.getText().c_str()));	
		
			return simple.render(true, false);// gl::Texture();
		}	

		void textFieldDraw(const TextItem& item, const ci::Vec2f& coords = Vec2f::zero())
		{
			gl::pushMatrices();
			gl::translate(coords);
			ci::TextLayout simple;			
			simple.setFont( item.getFont());
			simple.setColor(item.getColor() );
			simple.addLine(stringTools().cp1251_to_utf8(item.getText().c_str()));	
			gl::Texture render = gl::Texture( simple.render( true, false ) );
			gl::draw(render);
			gl::popMatrices();
			gl::color(Color::white());
		}

		void drawTextBox(const TextItem& item, const Color& color, const Vec2f& position, const Vec2f& size)
		{
			TextBox mTextBox;
			mTextBox.backgroundColor(color);
			mTextBox.setPremultiplied(false);
			mTextBox.setColor(item.getColor());
			mTextBox.setFont(item.getFont());			
			mTextBox.setSize(size);
			mTextBox.alignment(TextBox::CENTER);		
			mTextBox.setText(stringTools().cp1251_to_utf8(item.getText().c_str()));
		
			gl::pushMatrices();
			gl::translate(position);
			gl::draw(mTextBox.render());
			gl::popMatrices();
		}

		void drawTextBox(const TextItem& item, const Color& color, const Color& textColor, const Vec2f& position, const Vec2f& size)
		{
			TextBox mTextBox;
			mTextBox.backgroundColor(color);
			mTextBox.setPremultiplied(false);
			mTextBox.setColor(textColor);
			mTextBox.setFont(item.getFont());
			mTextBox.setSize(size);
			mTextBox.alignment(TextBox::CENTER);
			mTextBox.setText(stringTools().cp1251_to_utf8(item.getText().c_str()));

			gl::pushMatrices();
			gl::translate(position);
			gl::draw(mTextBox.render());
			gl::popMatrices();
		}
	};

	// helper function(s) for easier access 
	inline TextTools&	textTools() { return TextTools::getInstance(); };
}