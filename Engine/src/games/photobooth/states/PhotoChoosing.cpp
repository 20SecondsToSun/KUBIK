#include "PhotoChoosing.h"

using namespace std;
using namespace kubik;
using namespace ci::signals;
using namespace ci::app;
using namespace kubik::games;

PhotoChoosing::PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage)
	:photoStorage(photoStorage),
	photoFiltersStartY(350.0f)
{	
	Vec2f pos[PHOTOS_NUM] = 
	   {Vec2f(96.0f, 439.0f),
		Vec2f(404.0f, 439.0f),
		Vec2f(711.0f, 439.0f), 
		Vec2f(96.0f, 864.0f),
		Vec2f(404.0f, 864.0f)};

	for (int i = 0; i < PHOTOS_NUM; i++)	
		photoBtns.push_back(PhotoContainerRef(new PhotoContainer(i, settings->getTexture("galka"), settings->getTexture("ramka"), pos[i])));		
	
	okBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("okBtn"), Vec2f(780.0f, 960.0f)));	

	reset(settings);
}		

void PhotoChoosing::start()
{
	drawLocation = false;

	for (auto btn : photoBtns)		
		btn->setSelected(false);

	selectedNum = 0;

	float startY = 521;//321.0f;
	float shiftY = 363.0f;

	preloaderPos = Vec2f(0.5f * (getWindowWidth() - preloader.getWidth()), startY + 381.0f);
	preloaderAlpha = 1.0f;

	filterSelected(4);

	auto title1   = settings->getTextItem(PhtTextID::PRELOAD_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::PRELOAD_TEXT2);
	auto title3   = settings->getTextItem(PhtTextID::PHOTO_FILTERS);
	

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);
	tex3 = textTools().getTextField(title3);

	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), startY);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), startY + 124.0f);
	title3Pos = Vec2f(108.0f, photoFiltersStartY + 74.0f);

	timeline().apply( &title1Pos, ci::Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), startY - shiftY), 0.7f, ci::EaseOutCubic()).delay(2.0f);
	timeline().apply( &title2Pos, ci::Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), startY + 124.0f - shiftY), 0.7f, ci::EaseOutCubic()).delay(2.0f);

	timeline().apply( &preloaderPos, ci::Vec2f(0.5f * (getWindowWidth() - preloader.getWidth()), startY + 481.0f), 0.7f, ci::EaseOutCubic())
		.finishFn(bind( &PhotoChoosing::hidePreloaderComplete, this)).delay(2.0f);
	timeline().apply( &preloaderAlpha, 0.0f, 0.7f, ci::EaseOutCubic()).delay(2.0f);
}

void PhotoChoosing::stop()
{
	for (auto btn : photoBtns)		
		btn->disconnectEventHandler();

	for (auto fbtn : filterBtns)		
		fbtn->disconnectEventHandler();	

	okBtn->disconnectEventHandler();
}

void PhotoChoosing::hidePreloaderComplete()
{
	auto title1   = settings->getTextItem(PhtTextID::CHOOSE_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::CHOOSE_TEXT2);

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);

	float shiftY = 363.0f;
	float startY = 521.0f - shiftY;
	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), startY);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), startY + 124.0f - 45.0f);

	drawLocation = true;

	for (auto btn : photoBtns)		
		btn->connectEventHandler(&PhotoChoosing::photoChoosed, this);

	for (auto fbtn : filterBtns)		
		fbtn->connectEventHandler(&PhotoChoosing::filterChanged, this);	
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
		okBtn->connectEventHandler(&PhotoChoosing::okBtnClicked, this);	
	else
		okBtn->disconnectEventHandler();
}

void PhotoChoosing::filterChanged(EventGUIRef& event)
{
	auto eventref = static_pointer_cast<FilterChangedEvent>(event);	
	auto id = eventref->getID();
	console()<<"filter choosed:::  "<<id<<endl;
	filterSelected(id);
}

void PhotoChoosing::filterSelected(int id)
{
	float fwidth = 109.0f;
	float shiftX = 15.0f;
	float bigfwidth = 137.0f;
	float fulSize = (filterBtns.size() - 1.0f) * fwidth + (filterBtns.size() - 1.0f) * shiftX  + bigfwidth;

	Vec2f startVec = Vec2f( 0.5f * (getWindowWidth() - fulSize), photoFiltersStartY + 167.0f);
	Vec2f offset = startVec;

	for (int i = 0; i < filterBtns.size(); i++)
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
	nextLocationSignal();
}

void PhotoChoosing::reset(PhotoboothSettingsRef _settings) 
{
	settings = _settings;		
	preloader = settings->getTexture("preloader");	

	filterBtns.clear();

	auto filters = settings->getOnFilters();	

	float fwidth = 109.0f;
	float shiftX = 15.0f;
	float fullSize = filters.size() * fwidth + (filters.size() - 1.0f) * shiftX;

	Vec2f startVec = Vec2f( 0.5f * (getWindowWidth() - fullSize), photoFiltersStartY + 167.0f);

	DesignData designdata = settings->getPhotoFiltersPreview();		

	for (int i = 0; i < filters.size(); i++)
	{	
		auto position = startVec + Vec2f((shiftX + fwidth) * i, 0.0f);
		filterBtns.push_back(FilterSmallButtonRef(new FilterSmallButton(position, i, filters[i].getText(), settings->getFont("introBook12"), settings->getFont("introBook14"))));
	}
}	

void PhotoChoosing::update()
{
	
}

void PhotoChoosing::draw()
{
	gl::draw(tex1, title1Pos);
	gl::draw(tex2, title2Pos);

	gl::color(ColorA(1,1,1,preloaderAlpha));
	gl::draw(preloader, preloaderPos);
	gl::color(Color::white());

	if(drawLocation)
	{
		//drawPhotoPreview();
		drawPhotoFilters();
	}
}

void PhotoChoosing::drawPhotoPreview()
{
	for (auto btn : photoBtns)
		btn->draw();	

	if (selectedNum == MAX_SELECT)
		okBtn->draw();
}

void PhotoChoosing::drawPhotoFilters()
{
	gl::color(ColorA::hexA(0x26e4cf97));
	gl::drawLine(Vec2f(0.0f, photoFiltersStartY), Vec2f(getWindowWidth(), photoFiltersStartY));

	gl::color(Color::hex(0xe4cf97));
	gl::draw(tex3, title3Pos);

	gl::color(Color::white());
	for (auto filter : filterBtns)
		filter->draw();		
}