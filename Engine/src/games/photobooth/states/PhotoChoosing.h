#pragma once

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
		Surface image;
		Font font;

		int canSelectCount;
		int nowSelectCount;

		PhotoButton *lastSelectedPhotoButton;

		vector<PhotoButtonRef> photoBtns;
		MenuButtonRef	nextButton;
		PhotoStorageRef photoStorage;
		vector<Surface> thumbs;

	public:
		PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage):photoStorage(photoStorage)
		{			
			reset(settings);
		}

		~PhotoChoosing()
		{
			//disconnect buttons
		}

		void start() override
		{
			console()<<"start PhotoChoosing"<<endl;			

			canSelectCount = 0;//settings->getData().photoNum;			
			nowSelectCount = canSelectCount;			
			thumbs = photoStorage->getChoosingThumbs();
			
			photoBtns.clear();

			float shift = 0.0f;
			for (size_t i = 0; i < thumbs.size(); i++)
			{			
				createPhotoButton(i, Vec2f(shift, 200.0f));
				shift += thumbs[i].getWidth() + 20;

				bool value = (i < (size_t)canSelectCount);
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

			nextButton = MenuButtonRef(new MenuButton(Rectf(800.0f, 700.0f, 900.0f, 800.0f), "ÄÀËÅÅ", font, (GameId)1));	
			//connect_once(nextButton->mouseUpSignal, bind(&PhotoChoosing::mouseUpNextListener, this, placeholders::_1));
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
			textTools().textFieldDraw("ÂÛÁÅÐÈÒÅ " + to_string(canSelectCount) + " ÔÎÒÎÃÐÀÔÈÈ", &font, Color::white(), Vec2i(10, 10));

			for (auto btn: photoBtns)
				btn->draw();

			nextButton->draw();
		}

		void mouseUpHandler(Vec2i vec) override
		{
			for (auto btn : photoBtns)		
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