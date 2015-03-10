#include "PhotoFilter.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;
using namespace kubik;
using namespace kubik::games;

PhotoFilter::PhotoFilter(PhotoboothSettingsRef settings)
{
	reset(settings);				
}

void PhotoFilter::start()
{
	console()<<"start PhotoFilter "<<getCountFiltersOn()<<endl;
	//cameraCanon().startLiveView();

	if (getCountFiltersOn() <= 1)
		nextLocationSignal();
	else
	{
		for (auto filter: filterBtns)
			filter->connectEventHandler(&PhotoFilter::photoFilterSelect, this);
	}
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
	console()<<"filter id"<<eventref->getFilterId()<<endl;
	nextLocationSignal();
};

void PhotoFilter::reset(PhotoboothSettingsRef set)
{		
	settings = set;							

	auto title1   = settings->getTextItem(PhtTextID::FILTER_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::FILTER_TEXT2);

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);

	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), 172.0f);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), 258.0f);				

	filters = settings->getOnFilters();
	filterBtns.clear();
	removeAllChildren();

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
}

void PhotoFilter::createfilters3()
{
	float x = 9, y = 467;
	float width = 705, height = 1255;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 720;
	width = 360, height = 625;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	y = 1097;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));
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

	x = 9; y = 1106;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));
}

void PhotoFilter::createfilters5()
{	
	float x = 9, y = 467;
	float width = 528, height = 633;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));
	///
	x = 9; y = 1106;
	width = 351, height = 625;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	x += width + 4;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));
}

void PhotoFilter::createfilters6()
{	
	float x = 9, y = 467;
	float width = 351, height = 625;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));		

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 9 ; y = 1097;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));

	x += width + 5;
	buttonArea = Rectf(x, y, x + width, y + height);				
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));
}

void PhotoFilter::update()
{
	//cameraCanon().update();
}

void PhotoFilter::draw()
{
	gl::color(ci::Color::hex(0x060a0e));
	gl::drawSolidRect(getWindowBounds());
	gl::color(ci::Color::white());

	gl::draw(tex1, title1Pos);
	gl::draw(tex2, title2Pos);

	Sprite::draw();
}		