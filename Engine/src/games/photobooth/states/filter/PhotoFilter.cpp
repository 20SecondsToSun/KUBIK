#include "PhotoFilter.h"
#include "shaders/ShaderTool.h"

using namespace std;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;
using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::photobooth;

PhotoFilter::PhotoFilter(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage)
	:IGameLocation(), 
	photoStorage(photoStorage)
{
	setImageSizeParams();
	reset(settings);
}

void PhotoFilter::start()
{
	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Start ~~~");
	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Filers Size : " + to_string(getCountFiltersOn()) + " ~~~");

	cameraCanon().startLiveView();
	shaders::imagefilters::shadertool().setdefaultValues();

	if (getCountFiltersOn() <= 1)
	{
		skipLocation();
	}		
	else
	{
		initStartAnimation();
	}		

	state = TITLE_ANIM;
}

void PhotoFilter::skipLocation()
{
	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Skip - One Filters Set ~~~");

	nextLocationSignal();
}

void PhotoFilter::initStartAnimation()
{
	{//title animation start
		titleAlpha = 0.1f;
		titleAnimPosition = titlePos - Vec2f(0.0f, 70.0f);
		timeline().apply(&titleAlpha, 1.0f, animShowTitleTime, EaseOutCubic());
		timeline().apply(&titleAnimPosition, titlePos, animShowTitleTime, EaseOutCubic()).finishFn(bind(&PhotoFilter::titleAnimComplete, this));
	}

	{// filters animation init
		float delay = animShowTitleTime;
		float showingTime = 1.1f;
		for (auto filter : filterBtns)
		{
			filter->showAnimate(0.0f, 1.0f, showingTime, delay);
			delay += 0.1f;
		}
		delaycall(bind(&PhotoFilter::showAnimationComplete, this), showingTime + delay);// TODO some on complete problems
	}
}

void PhotoFilter::titleAnimComplete()
{
	state = UPDATE;
	//showAnimationComplete();
}

void PhotoFilter::showAnimationComplete()
{
	callback(COMPLETE_ANIM);

	for (auto filter : filterBtns)
	{
		filter->connectEventHandler(&PhotoFilter::photoFilterSelect, this);
	}
}

void PhotoFilter::stop()
{
	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Stop ~~~");

	stopAllTweens();

	for (auto filter : filterBtns)
	{
		filter->disconnectEventHandler();
	}		
}

void PhotoFilter::photoFilterSelect(EventGUIRef& event)
{
	auto eventref = static_pointer_cast<PhotoFilterEvent>(event);
	auto filterID = eventref->getFilterId();
	photoStorage->setSelectedFilter(filterID);

	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Filters ID Set " + to_string(filterID ) + " ~~~");
	dbRecord->FilterNum = filterID;

	callback(BEGIN_ANIM);
	nextLocationSignal();
};

void PhotoFilter::reset(ISettingsRef settings)
{
	logger().log("~~~ Photobooth.SubLocation PhotoFilters.Reset ~~~");

	filterBtns.clear();
	removeAllChildren();

	IGameLocation::reset(settings);
	title = settings->getTexture("filterTitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), titlePositionY - title.getHeight() * 0.5f);
	auto setphoto = static_pointer_cast<PhotoboothSettings>(settings);
	filters = setphoto->getOnFilters();

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

	for (auto filter : filterBtns)
	{
		addChild(filter);
	}		
}

int PhotoFilter::getCountFiltersOn()
{
	auto setphoto = static_pointer_cast<PhotoboothSettings>(settings);
	return setphoto->getOnFilters().size();
}

void PhotoFilter::createfilters2()
{
	float x = 9.0f, y = 520.0f;
	float width = 528.0f, height = 938.0f;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 542;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	for (auto filter : filterBtns)
	{
		filter->setSizeID(filter_2);
	}		

	filterUpdate = &PhotoFilter::filterUpdate2;
}

void PhotoFilter::createfilters3()
{
	float x = 9.0f, y = 467.0f;
	float width = 705.0f, height = 1255.0f;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea, filter_3_1)));

	x = 720.0f;
	width = 360.0f, height = 625.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea, filter_3_2)));

	y = 1097.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea, filter_3_2)));

	filterUpdate = &PhotoFilter::filterUpdate3;
}

void PhotoFilter::createfilters4()
{
	float x = 9.0f, y = 467.0f;
	float width = 528.0f, height = 633.0f;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x = 542.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	x = 9.0f; y = 1104.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 542.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	for (auto filter : filterBtns)
	{
		filter->setSizeID(filter_4);
	}		

	filterUpdate = &PhotoFilter::filterUpdate4;
}

void PhotoFilter::createfilters5()
{
	float x = 9.0f, y = 467.0f;
	float width = 528.0f, height = 633.0f;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea, filter_5_1)));

	x = 542.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea, filter_5_1)));
	///
	x = 9.0f; y = 1104.0f;
	width = 351.0f, height = 625.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea, filter_5_2)));

	x += width + 5.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea, filter_5_2)));

	x += width + 4.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea, filter_5_2)));

	filterUpdate = &PhotoFilter::filterUpdate5;
}

void PhotoFilter::createfilters6()
{
	float x = 9.0f, y = 467.0f;
	float width = 351.0f, height = 621.0f;
	Rectf buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[0].getID(), buttonArea)));

	x += width + 5.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[1].getID(), buttonArea)));

	x += width + 5.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[2].getID(), buttonArea)));

	x = 9.0f; y = 1093.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[3].getID(), buttonArea)));

	x += width + 5.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));

	x += width + 5.0f;
	buttonArea = Rectf(x, y, x + width, y + height);
	filterBtns.push_back(FilterButtonRef(new FilterButton(filters[4].getID(), buttonArea)));

	for (auto filter : filterBtns)
	{
		filter->setSizeID(filter_6);
	}		

	filterUpdate = &PhotoFilter::filterUpdate6;
}

void PhotoFilter::filterUpdate2()
{
	auto params = updateTextures[filter_2].params;
	updateTextures[filter_2].texture = getCanonCameraTexture(params);
}

void PhotoFilter::filterUpdate3()
{
	auto params = updateTextures[filter_3_1].params;
	updateTextures[filter_3_1].texture = getCanonCameraTexture(params);

	params = updateTextures[filter_3_2].params;
	updateTextures[filter_3_2].texture = getCanonCameraTexture(params);
}

void PhotoFilter::filterUpdate4()
{
	auto params = updateTextures[filter_4].params;
	updateTextures[filter_4].texture = getCanonCameraTexture(params);
}

void PhotoFilter::filterUpdate5()
{
	auto params = updateTextures[filter_5_1].params;
	updateTextures[filter_5_1].texture = getCanonCameraTexture(params);

	params = updateTextures[filter_5_2].params;
	updateTextures[filter_5_2].texture = getCanonCameraTexture(params);
}

void PhotoFilter::filterUpdate6()
{
	auto params = updateTextures[filter_6].params;
	updateTextures[filter_6].texture = getCanonCameraTexture(params);
}

void PhotoFilter::update()
{
	if (state != UPDATE) return;

	cameraCanon().update();

	(this->*filterUpdate)();

	for (auto filter : filterBtns)
	{
		filter->setTexture(updateTextures[(sizeID)filter->getSizeID()].texture);
	}		
}

void PhotoFilter::draw()
{
	fillBg();
	drawTitle();
	Sprite::draw();
}

ci::gl::Texture PhotoFilter::getCanonCameraTexture(const TexParams& params)
{
	return cameraCanon().getTexture(params.sizex, params.sizey, params.offsetx, params.offsety, params.scale);
}

void PhotoFilter::setImageSizeParams()
{
	TexParams params;

	float scale = 528.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 1056.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = 0.0f;
	params.scale = scale;
	updateTextures[filter_2].params = params;

	scale = 705.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 1056.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = 0.0f;
	params.scale = scale;
	updateTextures[filter_3_1].params = params;

	scale = 351.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 1056.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = 0.0f;
	params.scale = scale;
	updateTextures[filter_3_2].params = params;

	scale = 528.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 714.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = -113.0f;
	params.scale = scale;
	updateTextures[filter_4].params = params;

	scale = 528.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 714.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = -113.0f;
	params.scale = scale;
	updateTextures[filter_5_1].params = params;

	scale = 351.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 1056.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = 0.0f;
	params.scale = scale;
	updateTextures[filter_5_2].params = params;

	scale = 351.0f / 596.0f;
	params.sizex = 596.0f * scale;
	params.sizey = 1056.0f * scale;
	params.offsetx = 0.0f;
	params.offsety = 0.0f;
	params.scale = scale;
	updateTextures[filter_6].params = params;
}

void PhotoFilter::stopAllTweens()
{
	for (auto filter : filterBtns)
	{
		filter->stopAllTweens();
	}		

	IGameLocation::stopAllTweens();
}