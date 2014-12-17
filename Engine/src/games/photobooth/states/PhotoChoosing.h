#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "PhotoButton.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;

namespace kubik
{
	class PhotoChoosing:public IPhotoboothLocation
	{
		gl::Texture fon;
		ci::Font font;

		int canSelectCount;
		int nowSelectCount;
		PhotoButton* lastSelectedPhotoButton;

		vector<shared_ptr<PhotoButton>> photoBtns;
		shared_ptr<MenuButton> nextButton;

	public:

		PhotoChoosing(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);
		};

		~PhotoChoosing()
		{

		};

		void start() override
		{
			console()<<"start PhotoChoosing"<<endl;

			for (int i = 0; i < photoBtns.size(); i++)
			{
				bool value = i < canSelectCount;
				photoBtns[i]->setSelection(value);
			}

			canSelectCount = settings->getData().photoNum;
			nowSelectCount = canSelectCount;
			lastSelectedPhotoButton = photoBtns[canSelectCount - 1].get();
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon  =  settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();	

			photoBtns.clear();

			for (int i = 0; i < settings->getPhotoShots(); i++)
				createPhotoButton(i, to_string(i), i);

			nextButton = shared_ptr<MenuButton>(new MenuButton(1, Rectf(800.0f, 500.0f, 900.0f, 600.0f), "ÄÀËÅÅ", font));	
			connect_once(nextButton->mouseUpSignal, bind(&PhotoChoosing::mouseUpNextListener, this, std::placeholders::_1));

			start();
		}	

		void createPhotoButton(int id, std::string text, int i)
		{
			shared_ptr<PhotoButton> button = shared_ptr<PhotoButton>(new PhotoButton(id, getButtonArea(i), text, font));	
			connect_once(button->mouseUpSignal, bind(&PhotoChoosing::mouseUpListener, this, std::placeholders::_1));
			photoBtns.push_back(button);
		}

		Rectf getButtonArea(int i) 
		{
			float x      = 200.0f * i;
			float y      = 200.0f;
			float width  = 100.0f;
			float height = 100.0f;

			return Rectf(x, y, x + width, y + height);
		}

		void mouseUpListener(PhotoButton& button)
		{
			if(button.isSelect())
				nowSelectCount++;
			else
				nowSelectCount--;

			if(nowSelectCount > canSelectCount)
			{
				lastSelectedPhotoButton->setSelection(false);

				nowSelectCount = canSelectCount;
			}		

			lastSelectedPhotoButton = &button;
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());		
			textTools().textFieldDraw("ÂÛÁÅÐÈÒÅ " + to_string(canSelectCount) +" ÔÎÒÎÃÐÀÔÈÈ", &font, Vec2f(100.0f, 100.0f), Color::white());

			for (auto btn: photoBtns)
				btn->draw();

			nextButton->draw();
		}

		void mouseUpHandler(Vec2i vec) override
		{
			for (auto btn: photoBtns)		
				btn->mouseUpHandler(vec);

			nextButton->mouseUpHandler(vec);
		}

		void mouseUpNextListener(MenuButton& button)
		{
			nextLocationSignal();
		}		
	};
}