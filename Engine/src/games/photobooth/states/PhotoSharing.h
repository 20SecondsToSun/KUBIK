#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "ShareButton.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;

namespace kubik
{
	class PhotoSharing:public IPhotoboothLocation
	{
		gl::Texture fon;
		ci::Font font;

		enum shareID
		{
			FACEBOOK,
			VKONTAKTE,
			TWITTER,
			EMAIL
		};

		vector<shared_ptr<ShareButton>> shareBtns;

		public:

		PhotoSharing(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);		
		};

		~PhotoSharing()
		{

		};

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon = settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();

			PhotoboothSettings::PhotoboothDataStruct data = settings->getData();
			
			shareBtns.clear();
			int i = 0;

			if (data.sharing.isFacebook)
				createShareButton(FACEBOOK, "FACEBOOK", i++);

			if (data.sharing.isVkotakte)
				createShareButton(VKONTAKTE, "VKONTAKTE", i++);

			if (data.sharing.isTwitter)
				createShareButton(TWITTER, "TWITTER", i++);

			if (data.sharing.isEmail)
				createShareButton(EMAIL, "EMAIL", i++);
		}

		void createShareButton(shareID id, string text, int i)
		{
			shared_ptr<ShareButton> button = shared_ptr<ShareButton>(new ShareButton(id, getButtonArea(i), text, font));	
			connect_once(button->mouseUpSignal, bind(&PhotoSharing::mouseUpListener, this, std::placeholders::_1));
			shareBtns.push_back(button);
		}

		Rectf getButtonArea(int i) 
		{
			float x      = 300.0f * i;
			float y      = 300.0f;
			float width  = 200.0f;
			float height = 200.0f;

			return Rectf(x, y, x + width, y + height);
		}

		void mouseUpListener(ShareButton& button)
		{
			nextLocationSignal();
		}

		void start() override
		{
			console()<<"start PhotoSharing"<<endl;
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("SHARING", &font, Vec2f(100.0f, 100.0f), Color::white());

			for (auto btn: shareBtns)
				btn->draw();
		}

		void mouseUpHandler( Vec2i vec) override
		{
			for (auto btn: shareBtns)		
				btn->mouseUpHandler(vec);
		}
	};
}