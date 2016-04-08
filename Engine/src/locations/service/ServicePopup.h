#pragma once

#include "IScreen.h"
#include "TextTools.h"

namespace kubik
{
	class ServicePopup:public IScreen
	{
	public:	
		ServicePopup();
		void draw();
		void init(kubik::config::ISettingsRef config) override;
		void reset() override;
		void setMessage(const char* msg);

	private:	
		std::string msg;
		ci::Font font;
		ci::Vec2f textTexPos;
		ci::gl::Texture textTex;
	};

	typedef std::shared_ptr<ServicePopup> ServicePopupRef;
}