#pragma once

#include "IScreen.h"
#include "TextTools.h"

namespace kubik
{
	class ServicePopup:public IScreen
	{
	public:	
		ServicePopup()
		{	
			string path = getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf";// TODO FONT in Resource
			DataSourceRef ref = loadFile(path);
			font = Font(ref, 30);	
		}

		void draw()
		{	
			gl::color(Color::white());
			gl::drawSolidRect(getWindowBounds());		
			gl::draw(textTex, textTexPos);
		}	

		void init(ISettingsRef config) override
		{

		}

		void reset() override
		{

		};

		void setMessage(const char* msg)
		{			
			this->msg = string(msg);

			textTex = textTools().getTextField(msg, &font, ColorA(1, 0, 0, 1));
			
			float shiftX = (getWindowWidth() - textTex.getWidth()) * 0.5f;
			float shiftY = (getWindowHeight() - textTex.getHeight()) * 0.5f;
			textTexPos = Vec2f(shiftX, shiftY);
		}

	private:	
		std::string msg;
		ci::Font font;
		ci::Vec2f textTexPos;
		ci::gl::Texture textTex;
	};

	typedef shared_ptr<ServicePopup> ServicePopupRef;
}