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

		void textFieldDraw(std::string text,ci::Font *font,  ColorA color, Vec2f coords = Vec2f::zero())
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

		gl::Texture  getTextField(std::string text,ci::Font* font, ColorA color)
		{	
			ci::TextLayout simple;			
			simple.clear(ColorA(1,1,1,0));
			simple.setFont( *font );
			simple.setColor(color );
			simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));			
			return gl::Texture( simple.render( true, false ) );
		}

		gl::Texture  getTextField(const char* text,ci::Font* font, ColorA color)
		{		
			ci::TextLayout simple;
			simple.clear(ColorA(1,1,1,0));
			simple.setFont( *font );
			simple.setColor(color );
			simple.addLine(stringTools().cp1251_to_utf8(text));			
			return gl::Texture( simple.render( true, false ) );
		}

		gl::Texture getTextField(TextItem item, bool isCentered = false, float offset = 0)
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

			return gl::Texture( simple.render( true, false ) );
		}	

		void textFieldDraw(TextItem item, Vec2f coords = Vec2f::zero())
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
	};

	// helper function(s) for easier access 
	inline TextTools&	textTools() { return TextTools::getInstance(); };
}