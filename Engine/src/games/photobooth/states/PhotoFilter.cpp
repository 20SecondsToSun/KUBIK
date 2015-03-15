#include "PhotoFilter.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;
using namespace kubik;
using namespace kubik::games::photobooth;

PhotoFilter::PhotoFilter(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage)
	:photoStorage(photoStorage)
{
	reset(settings);				
}

void PhotoFilter::start()
{
	console()<<"start PhotoFilter "<<getCountFiltersOn()<<endl;
	cameraCanon().startLiveView();

	if (getCountFiltersOn() <= 1)
		nextLocationSignal();
	else
	{
		titleAlpha = 0.1f;
		titleAnimPosition = Vec2f(titlePos.x, titlePositionY - 70.0f);
		timeline().apply( &titleAlpha, 1.0f, animShowTitleTime,  EaseOutCubic());
		timeline().apply( &titleAnimPosition, Vec2f(titlePos.x, titlePositionY), animShowTitleTime,  EaseOutCubic()).finishFn(bind(&PhotoFilter::showAnimationComplete, this));	
		
		float delay = 0.0f;
		for (auto filter: filterBtns)
		{
			filter->showAnimate(1.5f, delay);
			delay += 0.1;
		}
	}
}

void PhotoFilter::showAnimationComplete()
{
	for (auto filter: filterBtns)
		filter->connectEventHandler(&PhotoFilter::photoFilterSelect, this);
}

void PhotoFilter::stop()
{
	console()<<"stop photofilter  "<<endl;
	for (auto filter: filterBtns)
		filter->disconnectEventHandler();
}

void PhotoFilter::photoFilterSelect(EventGUIRef& event)
{ 
	auto eventref = static_pointer_cast<PhotoFilterEvent>(event);
	auto filterID = eventref->getFilterId();
	photoStorage->setSelectedFilter(filterID);

	console()<<"filter id"<<filterID<<endl;
	nextLocationSignal();
};

void PhotoFilter::reset(PhotoboothSettingsRef settings)
{
	filterBtns.clear();
	removeAllChildren();

	IPhotoboothLocation::reset(settings);	
	title	 = settings->getTexture("filterTitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), titlePositionY - title.getHeight() * 0.5f);
	filters  = settings->getOnFilters();	

	switch (filters.size())
	{
	case 2:
		createfilters2();
		break;
	case 3:
		createfilters3();
		break;
	case 4:
		createfilters4();
		break;
	case 5:
		createfilters5();
		break;
	case 6:
		createfilters6();
		break;
	default:
		break;
	}

	for (auto filter: filterBtns)			
		addChild(filter);
}	

int PhotoFilter::getCountFiltersOn()
{
	return settings->getOnFilters().size();
}

void PhotoFilter::createfilters2()
{
	float x = 9, y = 520;
	float width = 528, height = 938;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	for (auto filter: filterBtns)	
		filter->setSizeID(filter_2);

	filterUpdate = &PhotoFilter::filterUpdate2;
}

void PhotoFilter::createfilters3()
{
	float x = 9, y = 467;
	float width = 705, height = 1255;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea, filter_3_1)));

	x = 720;
	width = 360, height = 625;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea, filter_3_2)));

	y = 1097;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea, filter_3_2)));
	
	filterUpdate = &PhotoFilter::filterUpdate3;
}

void PhotoFilter::createfilters4()
{	
	float x = 9, y = 467;
	float width = 528, height = 633;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	x = 9; y = 1104;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	for (auto filter: filterBtns)	
		filter->setSizeID(filter_4);

	filterUpdate = &PhotoFilter::filterUpdate4;
}

void PhotoFilter::createfilters5()
{	
	float x = 9, y = 467;
	float width = 528, height = 633;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea, filter_5_1)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea, filter_5_1)));
	///
	x = 9; y = 1104;
	width = 351, height = 625;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea, filter_5_2)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea, filter_5_2)));

	x += width + 4;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea, filter_5_2)));

	filterUpdate = &PhotoFilter::filterUpdate5;
}

void PhotoFilter::createfilters6()
{	
	float x = 9, y = 467;
	float width = 351, height = 621;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));		

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 9 ; y = 1093;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));

	for (auto filter: filterBtns)	
		filter->setSizeID(filter_6);

	filterUpdate = &PhotoFilter::filterUpdate6;
}

void PhotoFilter::filterUpdate2()
{
	float scale = 528.0f / 596.0f;
	updateTextures[filter_2] = cameraCanon().getTexture(596.0f * scale, 1056.0f * scale, 0.0f, 0.0f, scale);
}

void PhotoFilter::filterUpdate3()
{
	float scale = 705.0f / 596.0f;
	updateTextures[filter_3_1] = cameraCanon().getTexture(596.0f * scale, 1056.0f * scale, 0.0f, 0.0f, scale);
	scale = 351.0f / 596.0f;
	updateTextures[filter_3_2] = cameraCanon().getTexture(596.0f * scale, 1056.0f * scale, 0.0f, 0.0f, scale);
}

void PhotoFilter::filterUpdate4()
{
	 float scale = 528.0f / 596.0f;
	 updateTextures[filter_4] = cameraCanon().getTexture(596 * scale, 714.0f * scale, 0, -113, scale);
}

void PhotoFilter::filterUpdate5()
{
	float scale = 528.0f / 596.0f;
	 updateTextures[filter_5_1] = cameraCanon().getTexture(596 * scale, 714.0f * scale, 0, -113, scale);
	 scale = 351.0f / 596.0f;
	 updateTextures[filter_5_2] = cameraCanon().getTexture(596.0f * scale, 1056.0f * scale, 0.0f, 0.0f, scale);
}

void PhotoFilter::filterUpdate6()
{
	float scale = 351.0f / 596.0f;
	updateTextures[filter_6] = cameraCanon().getTexture(596 * scale, 1056 * scale, 0, 0, scale);
}

void PhotoFilter::update()
{
	cameraCanon().update();	

	(this->*filterUpdate)();

	for (auto filter: filterBtns)	
		filter->setTexture(updateTextures[(sizeID)filter->getSizeID()]);
}

void PhotoFilter::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, titleAlpha));
	gl::draw(title, titleAnimPosition);
	gl::color(Color::white());
	Sprite::draw();
}		