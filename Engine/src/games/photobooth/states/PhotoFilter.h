#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "PhotoboothSettings.h"
#include "FilterButton.h"
#include "CameraAdapter.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;

namespace kubik
{
	class PhotoFilter:public IPhotoboothLocation
	{		
		Texture fon;
		Font	font;
		vector<shared_ptr<FilterButton>> filterBtns;	

	public:	
		PhotoFilter(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);			
		}

		~PhotoFilter()
		{
			console()<<"photo filter destructor"<<endl;			
		}

		void start()
		{
			console()<<"start PhotoFilter "<<getCountFiltersOn()<<endl;
			cameraCanon().startLiveView();

			if (getCountFiltersOn() <= 1)
				nextLocationSignal();
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{		
			settings = _settings;
			fon		 =  settings->getTextures()["fon2"]->get();
			font	 =  settings->getFonts()["helvetica40"]->get();

			filterBtns.clear();

			if (getCountFiltersOn() > 1)
			{
				vector<int> filters = settings->getOnFilters();
				int i = 0;

				for (auto filter: filters)
				{
					float x      = 300.0f * (1 + i++);
					float y      = 300.0f;
					float width  = 200.0f;
					float height = 200.0f;

					Rectf buttonArea = Rectf(x, y, x + width, y + height);

					shared_ptr<FilterButton> button = shared_ptr<FilterButton>(new FilterButton(filter, buttonArea, to_string(filter), font));	
					connect_once(button->mouseUpSignal, bind(&PhotoFilter::mouseUpListener, this, placeholders::_1));
					filterBtns.push_back(button);
				}
			}
		}

		void mouseUpListener(FilterButton& button )
		{
			nextLocationSignal();
		}

		int getCountFiltersOn()
		{
			return settings->getOnFilters().size();
		}

		void update() override
		{
			cameraCanon().update();
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			textTools().textFieldDraw("ÂÛÁÎÐ ÔÈËÜÒÐÀ", &font, Vec2f(100.0f, 100.0f), Color::white());			

			for (auto btn: filterBtns)
			{
				gl::pushMatrices();
					float scale = btn->getWidth() / cameraCanon().getWidth();							
					gl::translate(btn->getPosition().x, btn->getPosition().y);
					gl::scale(scale, scale);
					cameraCanon().draw();
				gl::popMatrices();
				//btn->draw();
			}
		}

		void mouseUpHandler(Vec2i vec)
		{
			for (auto btn: filterBtns)		
				btn->mouseUpHandler(vec);
		}
	};
}