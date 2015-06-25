#include "PhotoChoosing.h"

using namespace std;
using namespace kubik;
using namespace ci::signals;
using namespace ci::app;
using namespace ci;
using namespace kubik::games::photobooth;

PhotoChoosing::PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage)
	:photoStorage(photoStorage), photoFiltersStartY(1398.0f)
{
	photoPositions[0] = Vec2f(96.0f,  439.0f);
	photoPositions[1] = Vec2f(404.0f, 439.0f);
	photoPositions[2] = Vec2f(711.0f, 439.0f);
	photoPositions[3] = Vec2f(96.0f,  864.0f);
	photoPositions[4] = Vec2f(404.0f, 864.0f);

	reset(settings);
}

void PhotoChoosing::start()
{
//#define debug
#ifdef loadfromfolder
	photoStorage->clear();
#endif

	if (photoStorage->empty())
		for (int i = 1; i < 6; i++)	
			photoStorage->loadDownloadedPhoto("c:\\projects\\cinder_0.8.6_vc2012\\apps\\KUBIK\\Engine\\vc2012\\Debug\\data\\photoDir\\IMG_000" + to_string(i) + ".JPG");

	state = CHOOSING;
	selectedNum = 0;
	okBtn->setAlpha(0.0f);

	for (auto btn : photoBtns)
		btn->setSelected(false);

	filterSelected(photoStorage->getSelectedFilter());

	photoStorage->createChoosingPreview();
	auto texs = photoStorage->getChoosingPreview();

	assert(texs.size() == photoBtns.size());

	for (unsigned int i = 0; i < photoBtns.size(); i++)
		photoBtns[i]->setPhoto(texs[i]);

	for (unsigned int i = 0; i < filterBtns.size(); i++)
		filterBtns[i]->setPhoto(texs[0]);

	setShaderForPreviews();
	
#ifdef debug
	okBtn->setAlpha(0.0f);
	reShotBtn->setAlpha(0.0f);

	for (unsigned int i = 0; i < photoBtns.size(); i++)
	photoBtns[i]->setAlpha(0.0f);

	for (unsigned int i = 0; i < filterBtns.size(); i++)
	filterBtns[i]->setAlpha(0.0f);

	titleAnimPosition = titlePos - Vec2f(0.0f, 170.0f);
	choosefonPosAnim = Vec2f(0.0f, choosefon.getHeight());
	titleFilterAlpha = 0.0f;
	titleAlpha = 0.0f;
	timeline().apply(&titleAlpha, 0.0f, 0.1f, 0.4f, EaseOutExpo())
	.finishFn(bind(&PhotoChoosing::initShowAnimationParams, this));
#else	
	initShowAnimationParams();
#endif

#undef debug;
}

void PhotoChoosing::initShowAnimationParams()
{
	float delay = 0.0f;

	{// title animation init
		titleAlpha = 0.1f;
		titleAnimPosition = titlePos - Vec2f(0.0f, 170.0f);
		timeline().apply(&titleAlpha, 1.0f, animShowTitleTime + 0.2f);
		timeline().apply(&titleAnimPosition, titlePos, animShowTitleTime, EaseOutExpo());
	}

	{// photoBtns anim init
		for (size_t i = 0; i < photoBtns.size(); i++)
		{
			//console() << "alphaupdate -------------------------------->" << endl;
			photoBtns[i]->showAnimate(0.0f, 1.0f, 0.6f, delay);
			photoBtns[i]->showPositionAnimate(Vec2f(0.0f, -100.0f), 0.6f, delay);
			delay += 0.1f;
		}
	}

	{// service buttons anim init
		okBtn->showAnimate(0.0f, 0.4f, 0.9f, delay);
		delay += 0.1f;

		reShotBtn->showAnimate(0.0f, 1.0f, 0.9f, delay);
		delay += 0.1f;
	}

	{// title filter anim init

		titleFilterAlpha = 0.0f;
		timeline().apply(&titleFilterAlpha, 0.0f, 1.0f, 0.6f).delay(delay);
	}

	{// additional fon anim init
		choosefonPosAnim = Vec2f(0.0f, choosefon.getHeight());
		timeline().apply(&choosefonPosAnim, Vec2f::zero(), animShowTitleTime + 0.1f, EaseOutExpo());
	}

	float showingTime = 0.7f;
	{// filterBtns anim init
		delay = 0.0f;
		for (unsigned int i = 0; i < filterBtns.size(); i++)
		{
			filterBtns[i]->showAnimate(0.0f, 1.0f, showingTime, delay);
			filterBtns[i]->showPositionAnimate(Vec2f(0, 200.0f), showingTime, delay);
			delay += 0.1f;
		}
	}

	delaycall(bind(&PhotoChoosing::showAnimationComplete, this), showingTime + delay);
}

void PhotoChoosing::showAnimationComplete()
{
	for (auto btn : photoBtns)
		btn->connectEventHandler(&PhotoChoosing::photoChoosed, this);

	for (auto fbtn : filterBtns)
		fbtn->connectEventHandler(&PhotoChoosing::filterChanged, this);

	reShotBtn->connectEventHandler(&PhotoChoosing::backToReshot, this);

	callback(COMPLETE_ANIM);
}

void PhotoChoosing::reset(PhotoboothSettingsRef settings)
{
	IPhotoboothLocation::reset(settings);

	setTitle();
	setPhotoButtonsDesign();
	setOkButtonDesign();
	setReshotButtonDesign();
	setFiltersData();
	setFiltersTitleDesign();
	setAdditionaBackgroung();
}

void PhotoChoosing::stop()
{
	stopAllTweens();
	clearDelaycall();

	for (auto btn : photoBtns)
		btn->disconnectEventHandler();

	for (auto fbtn : filterBtns)
		fbtn->disconnectEventHandler();

	okBtn->disconnectEventHandler();
	reShotBtn->disconnectEventHandler();
}

void PhotoChoosing::setShaderForPreviews()
{
	using namespace shaders::imagefilters;

	auto filterID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)filterID);

	for (unsigned int i = 0; i < photoBtns.size(); i++)
		photoBtns[i]->setShader(shader);
}

void PhotoChoosing::photoChoosed(EventGUIRef& event)
{
	auto eventref = static_pointer_cast<PhotoChoosedEvent>(event);
	auto id = eventref->getID();

	photoBtns[id]->setSelected(!photoBtns[id]->selected());

	selectedNum = 0;

	for (auto btn : photoBtns)
		if (btn->selected())
			selectedNum++;

	if (selectedNum > MAX_SELECT)
	{
		lastSelected->setSelected(false);
		selectedNum = MAX_SELECT;
	}

	lastSelected = photoBtns[id];

	if (selectedNum == MAX_SELECT)
	{
		okBtn->setAlpha(1.0f);
		okBtn->connectEventHandler(&PhotoChoosing::okBtnClicked, this);
	}
	else
	{
		okBtn->setAlpha(0.4f);
		okBtn->disconnectEventHandler();
	}
}

void PhotoChoosing::filterChanged(EventGUIRef& event)
{
	auto eventref = static_pointer_cast<FilterChangedEvent>(event);
	auto id = eventref->getID();

	photoStorage->setSelectedFilter(id);
	setShaderForPreviews();
	filterSelected(id);
}

void PhotoChoosing::backToReshot(EventGUIRef& event)
{
	stop();
	setLastScreenShot();

	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.8f, EaseOutCubic())
		.finishFn(bind(&PhotoChoosing::callback, this, RESHOT_LOC));

	state = ANIM_HIDE;
}

void PhotoChoosing::filterSelected(int id)
{
	float fwidth = 109.0f;
	float shiftX = 15.0f;
	float bigfwidth = 137.0f;
	float fulSize = (filterBtns.size() - 1.0f) * fwidth + (filterBtns.size() - 1.0f) * shiftX + bigfwidth;

	Vec2f startVec = Vec2f(0.5f * (getWindowWidth() - fulSize), photoFiltersStartY + 88.0f);
	Vec2f offset = startVec;

	for (unsigned int i = 0; i < filterBtns.size(); i++)
	{
		filterBtns[i]->setPosition(offset);

		if (id != filterBtns[i]->getID())
		{
			offset += Vec2f((shiftX + fwidth), 0.0f);
			filterBtns[i]->setCloseState();
		}
		else
		{
			offset += Vec2f((shiftX + bigfwidth), 0.0f);
			filterBtns[i]->setOpenState();
		}
	}
}

void PhotoChoosing::okBtnClicked(EventGUIRef& event)
{
	stop();
	setLastScreenShot();

	for (unsigned int i = 0; i < photoBtns.size(); i++)
		if (photoBtns[i]->selected())
			photoStorage->setSelectedID(i);

	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.8f, EaseOutCubic())
		.finishFn(bind(&PhotoChoosing::callback, this, NEXT_LOC));

	state = ANIM_HIDE;

	photoStorage->createPhotoTemplates(); // TODO OPTIMIZATION
	callback(BEGIN_ANIM);
}

void PhotoChoosing::update()
{

}

void PhotoChoosing::draw()
{
	fillBg();

	switch (state)
	{
	case PhotoChoosing::CHOOSING:
		gl::draw(choosefon, choosefonPos + choosefonPosAnim);
		drawTitle();
		drawPhotoPreview();
		drawPhotoFilters();
		break;

	case PhotoChoosing::ANIM_HIDE:
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(screenshot);
		gl::color(Color::white());
		break;
	}
}

void PhotoChoosing::drawPhotoPreview()
{
	for (auto btn : photoBtns)
		btn->draw();
	okBtn->draw();
	reShotBtn->draw();
}

void PhotoChoosing::drawPhotoFilters()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, titleFilterAlpha));
	gl::draw(titleFilter, titleFilterPos);
	gl::color(Color::white());

	for (auto filter : filterBtns)
		filter->draw();
}

void PhotoChoosing::setTitle()
{
	title = settings->getTexture("choosetitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 238.0f - title.getHeight() * 0.5f);
}

void kubik::games::photobooth::PhotoChoosing::setPhotoButtonsDesign()
{
	photoBtns.clear();
	for (int i = 0; i < PHOTOS_NUM; i++)
		photoBtns.push_back(PhotoContainerRef(new PhotoContainer(i, settings->getTexture("galka"), settings->getTexture("ramka"), photoPositions[i])));
}

void PhotoChoosing::setOkButtonDesign()
{
	okBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("okBtn")));
	okBtn->setPosition(Vec2f(photoPositions[2].x + 0.5f * (272.0f - okBtn->getWidth()), 990.0f - 0.5f * okBtn->getHeight()));
}

void PhotoChoosing::setReshotButtonDesign()
{
	reShotBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("reshotBtn")));
	reShotBtn->setPosition(Vec2f(photoPositions[2].x + 0.5f * (272.0f - reShotBtn->getWidth()), 1166.0f - 0.5f * reShotBtn->getHeight()));//1066
}

void PhotoChoosing::setFiltersData()
{
	filterBtns.clear();
	auto filters = settings->getOnFilters();
	float fwidth = 109.0f, shiftX = 15.0f;
	float fullSize = filters.size() * fwidth + (filters.size() - 1.0f) * shiftX;

	Vec2f startVec = Vec2f(0.5f * (getWindowWidth() - fullSize), photoFiltersStartY + 167.0f);

	DesignData designdata = settings->getPhotoFiltersPreview();

	for (unsigned int i = 0; i < filters.size(); i++)
	{
		auto position = startVec + Vec2f((shiftX + fwidth) * i, 0.0f);
		filterBtns.push_back(FilterSmallButtonRef(
			new FilterSmallButton(position, 
			filters[i].getID(),
			filters[i].getText(), 
			settings->getTexture("plashFilter"),
			fontStorage().getFont("Intro-Book12"),
			fontStorage().getFont("Intro-Book14"))));
	}
}

void PhotoChoosing::setFiltersTitleDesign()
{
	titleFilter = settings->getTexture("chooseTitleFilter");
	titleFilterPos = Vec2f(photoPositions[0].x, photoFiltersStartY - 0.5f * titleFilter.getHeight());
}

void PhotoChoosing::setAdditionaBackgroung()
{
	choosefon = settings->getTexture("choosefon");
	choosefonPos = Vec2f(0.0f, 1920.0f - choosefon.getHeight());
}

void PhotoChoosing::stopAllTweens()
{
	titleFilterPos.stop();
	choosefonPosAnim.stop();
	alphaAnim.stop();
	IPhotoboothLocation::stopAllTweens();
}