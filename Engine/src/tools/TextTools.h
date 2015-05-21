#pragma once

#include "StringTools.h"
#include "TextItem.h"
#include "cinder/cairo/Cairo.h"

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

		ci::Surface getTextField(TextItem item, bool isCentered = false, float offset = 0)
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

		void drawCairo(const TextItem& item, Color color, Vec2f position)
		{
			
		//	cairo::SurfaceImage srfImage(800, 100, false);
		//	cairo::Context ctx(srfImage);// cairo::createWindowSurface());
	
		//	Color color = Color::hex(0x01a7fb);
		//	ctx.setSourceRgb(color.r, color.g, color.b);
		//	ctx.paint();

		//	// Render the name of the font in the font itself

		////	static Font mFont = Font(loadFile(fs::path("c:\\projects\\cinder_0.8.6_vc2012\\apps\\KUBIK\\Engine\\vc2012\\Debug\\data\fonts\\\HelveticaLight.ttf")), 18);

		//	ctx.setFont(item.getFont());
		//	//ctx.setFontSize(44);
		//	ctx.moveTo(50, 50);	
		//	color = item.getColor();
		//	ctx.setSourceRgb(color.r, color.g, color.b);

		//	cairo::FontOptions* opt = new cairo::FontOptions();			
		//	ctx.getFontOptions(opt);
		//	ctx.setAntiAlias(cairo::ANTIALIAS_GRAY);
		//	//opt->setAntiAlias(cairo::ANTIALIAS_GRAY);
		//	//ctx.setFontOptions(opt);
		//	
		///*	CAIRO_ANTIALIAS_DEFAULT,
		//		CAIRO_ANTIALIAS_NONE,
		//		CAIRO_ANTIALIAS_GRAY,
		//		CAIRO_ANTIALIAS_SUBPIXEL*/
		//	//ctx.setAntiAlias(0);
		//	ctx.setSourceRgb(item.getColor().r, item.getColor().g, item.getColor().b);
		//	ctx.showText("TITLEdfsggfsgfhgfhdgfhQWERTYUIOPLKHGF");// stringTools().cp1251_to_utf8(item.getText().c_str()));
		//	ctx.stroke();

			

			//auto mSimpleTexture = gl::Texture(srfImage.getSurface());
			//gl::draw(mSimpleTexture);			

			// create a text box (rectangular text area)
			TextBox mTextBox;// = TextBox();
			// set font and font size
			mTextBox.backgroundColor(color);// Color::hex(0x01a7fb));
			mTextBox.setPremultiplied(false);
			mTextBox.setColor(item.getColor());
			mTextBox.setFont(item.getFont());			
			mTextBox.setSize(Vec2i(914, 50));			
			mTextBox.alignment(TextBox::CENTER);
			// break lines between words
			//mTextBox.setBoundary(Text::WORD);
			// adjust space between lines
			//mTextBox.setLineSpace(1.5f);

			// load a text and hand it to the text box
			mTextBox.setText(stringTools().cp1251_to_utf8(item.getText().c_str())); //"TITLEdfsggfsgfhgfhdgfhQWERTYUIOPLKHGF");
			// draw the text
			gl::pushMatrices();
			gl::translate(0, position.y);
			gl::draw(mTextBox.render());
			gl::popMatrices();
			//gl::drawString("TITLEdfsggfsgfhgfhdgfhQWERTYUIOPLKHGF", Vec2f(40, 40));
		}
	};

	// helper function(s) for easier access 
	inline TextTools&	textTools() { return TextTools::getInstance(); };
}