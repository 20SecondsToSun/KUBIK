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
	class PhotoChoosing: public IPhotoboothLocation
	{
		Texture fon;
		Font font;
		Surface image;
		int canSelectCount;
		int nowSelectCount;

		PhotoButton *lastSelectedPhotoButton;

		vector<PhotoButtonRef> photoBtns;
		MenuButtonRef nextButton;
		PhotoStorageRef photoStorage;
		vector<Surface> thumbs;

	public:
		PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage):photoStorage(photoStorage)
		{			
			reset(settings);
		};

		~PhotoChoosing()
		{

		};

		void start() override
		{
			console()<<"start PhotoChoosing"<<endl;			

			canSelectCount = settings->getData().photoNum;			
			nowSelectCount = canSelectCount;			
			thumbs = photoStorage->getChoosingThumbs();
			
			photoBtns.clear();

			float shift = 0.0f;
			for (size_t i = 0; i < thumbs.size(); i++)
			{			
				createPhotoButton(i, Vec2f(shift, 200.0f));
				shift += thumbs[i].getWidth() + 20;

				bool value = (i < canSelectCount);
				photoBtns[i]->setSelection(value);
			}	

			lastSelectedPhotoButton = photoBtns[canSelectCount - 1].get();		
		}

		void createPhotoButton(int id, Vec2f vec)
		{
			PhotoButtonRef button = PhotoButtonRef(new PhotoButton(id, thumbs[id], vec));	
			connect_once(button->mouseUpSignal, bind(&PhotoChoosing::mouseUpListener, this, placeholders::_1));
			photoBtns.push_back(button);
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			fon  =  settings->getTexture("fon1");
			font =  settings->getFont("helvetica40");				

			nextButton = MenuButtonRef(new MenuButton((game::id)1, Rectf(600.0f, 500.0f, 700.0f, 600.0f), "ÄÀËÅÅ", font));	
			connect_once(nextButton->mouseUpSignal, bind(&PhotoChoosing::mouseUpNextListener, this, placeholders::_1));
		}	

		void mouseUpListener(PhotoButton& button)
		{
			if(button.isSelect())
				nowSelectCount++;
			else
			{
				nowSelectCount--;
				button.setSelection(false);
			}

			if(nowSelectCount > canSelectCount)
			{
				lastSelectedPhotoButton->setSelection(false);

				nowSelectCount = canSelectCount;
			}		

			lastSelectedPhotoButton = &button;
		}

		void update() override
		{

		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			textTools().textFieldDraw("ÂÛÁÅÐÈÒÅ " + to_string(canSelectCount) + " ÔÎÒÎÃÐÀÔÈÈ", &font, Vec2f(10.0f, 10.0f), Color::white());

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
			storeSelectedItems();

			if(photoStorage->isChoosedRightCount(canSelectCount))
				nextLocationSignal();
		}

		void storeSelectedItems()
		{
			photoStorage->clearPhotoChosenIds();

			for (size_t i = 0; i < photoBtns.size(); i++)			
				if(photoBtns[i]->isSelect())
					photoStorage->setPhotosChoosenIds(i);			
		}
	};

	typedef	shared_ptr<PhotoChoosing> PhotoChoosingRef;
}