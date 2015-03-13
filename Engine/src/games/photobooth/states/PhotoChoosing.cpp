#include "PhotoChoosing.h"

using namespace std;
using namespace kubik;
using namespace ci::signals;
using namespace ci::app;
using namespace kubik::games;

PhotoChoosing::PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage)
	:photoStorage(photoStorage),
	photoFiltersStartY(100.0f)
{
	reset(settings);
}		

void PhotoChoosing::start()
{
	for (auto btn : photoBtns)		
		btn->setSelected(false);

	selectedNum = 0;

	filterSelected(photoStorage->getSelectedFilter());	

	//debug
	//photoStorage->clear();
	//for (int i = 1; i < 6; i++)
	//{
	//	photoStorage->loadDownloadedPhoto("c:\\projects\\cinder_0.8.6_vc2012\\apps\\KUBIK\\Engine\\vc2012\\Debug\\data\\photoDir\\IMG_000"+to_string(i)+".JPG");
	//}
	//

	photoStorage->createChoosingPreview();
	auto texs = photoStorage->getChoosingPreview();

	assert(texs.size() == photoBtns.size());

	for (int i = 0; i < photoBtns.size(); i++)		
		photoBtns[i]->setPhoto(texs[i]);

	for (int i = 0; i < filterBtns.size(); i++)		
		filterBtns[i]->setPhoto(texs[0]);	

	setShaderForPreviews();

	hidePreloaderComplete();
}

void PhotoChoosing::setShaderForPreviews()
{	
	using namespace shaders::imagefilters;

	auto fID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)fID);

	for (int i = 0; i < photoBtns.size(); i++)		
		photoBtns[i]->setShader(shader);
}

void PhotoChoosing::stop()
{
	for (auto btn : photoBtns)		
		btn->disconnectEventHandler();

	for (auto fbtn : filterBtns)		
		fbtn->disconnectEventHandler();	

	okBtn->disconnectEventHandler();
	reShotBtn->disconnectEventHandler();
}

void PhotoChoosing::hidePreloaderComplete()
{
	title = settings->getTexture("choosetitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 238.0f - title.getHeight()*0.5f);

	drawLocation = true;

	for (auto btn : photoBtns)		
		btn->connectEventHandler(&PhotoChoosing::photoChoosed, this);

	for (auto fbtn : filterBtns)		
		fbtn->connectEventHandler(&PhotoChoosing::filterChanged, this);	

	reShotBtn->connectEventHandler(&PhotoChoosing::backToReshot, this);	
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

	photoStorage->setSelectedFilter(id);
	setShaderForPreviews();
	filterSelected(id);
}

void PhotoChoosing::backToReshot(EventGUIRef& event)
{
	callback(RESHOT_LOC);
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
	for (int i = 0; i < photoBtns.size(); i++)	
		if(photoBtns[i]->selected())
			photoStorage->setSelectedID(i);	

	nextLocationSignal();
}

void PhotoChoosing::reset(PhotoboothSettingsRef settings) 
{
	Vec2f pos[PHOTOS_NUM] = 
	   {Vec2f(96.0f, 439.0f),
		Vec2f(404.0f, 439.0f),
		Vec2f(711.0f, 439.0f), 
		Vec2f(96.0f, 864.0f),
		Vec2f(404.0f, 864.0f)};

	IPhotoboothLocation::reset(settings);	

	photoBtns.clear();
	for (int i = 0; i < PHOTOS_NUM; i++)	
		photoBtns.push_back(PhotoContainerRef(new PhotoContainer(i, settings->getTexture("galka"), settings->getTexture("ramka"), pos[i])));		
	
	okBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("okBtn")));
	okBtn->setPosition(Vec2f(pos[2].x + 0.5f * ( 272.0f - okBtn->getWidth()), 990.0f - 0.5f * okBtn->getHeight()));

	reShotBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("reshotBtn")));
	reShotBtn->setPosition(Vec2f(pos[2].x + 0.5f * ( 272.0f - reShotBtn->getWidth()), 1066.0f - 0.5f * reShotBtn->getHeight()));//1166
	
	filterBtns.clear();
	auto filters   = settings->getOnFilters();	
	float fwidth   = 109.0f, shiftX = 15.0f;
	float fullSize = filters.size() * fwidth + (filters.size() - 1.0f) * shiftX;

	Vec2f startVec = Vec2f(0.5f * (getWindowWidth() - fullSize), photoFiltersStartY + 167.0f);

	DesignData designdata = settings->getPhotoFiltersPreview();		

	for (int i = 0; i < filters.size(); i++)
	{	
		auto position = startVec + Vec2f((shiftX + fwidth) * i, 0.0f);
		filterBtns.push_back(FilterSmallButtonRef(new FilterSmallButton(position, i+1, filters[i].getText(), settings->getFont("introBook12"), settings->getFont("introBook14"))));
	}

	titleFilter		  = settings->getTexture("chooseTitleFilter");
	titleFilterPos	  = Vec2f(pos[0].x, photoFiltersStartY - 0.5f * titleFilter.getHeight());

	choosefon		  = settings->getTexture("choosefon");
	choosefonPos	  = Vec2f(0.0f, getWindowHeight() - choosefon.getHeight());
}	

void PhotoChoosing::update()
{
	
}

void PhotoChoosing::draw()
{
	fillBg();
	gl::draw(choosefon, choosefonPos);

	gl::draw(title, titlePos);

	if(drawLocation)
	{
		drawPhotoPreview();
		drawPhotoFilters();
	}
}

void PhotoChoosing::drawPhotoPreview()
{
	for (auto btn : photoBtns)
		btn->draw();	

	if (selectedNum == MAX_SELECT)
		okBtn->draw();

	reShotBtn->draw();
}

void PhotoChoosing::drawPhotoFilters()
{
	gl::draw(titleFilter, titleFilterPos);

	gl::color(Color::white());
	for (auto filter : filterBtns)
		filter->draw();		
}