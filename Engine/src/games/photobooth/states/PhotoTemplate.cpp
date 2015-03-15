#include "states/PhotoTemplate.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::app;
using namespace ci;

PhotoTemplate::PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage):photoStorage(photoStorage)
{		
	reset(settings);			
}

void PhotoTemplate::start()
{
	console()<<"start PhotoTemplate--------------------------"<<endl;
	photoStorage->createPhotoTemplates();

	using namespace shaders::imagefilters;
	auto fID = photoStorage->getSelectedFilter();
	auto shader = shadertool().get((ShaderTool::FilterType)fID);

	for (auto templ : templatebtns)	
	{
		templ->connectEventHandler(&PhotoTemplate::photoTemplateChoose, this);		
		templ->activateListeners();
		templ->setSelected(false);
		templ->setPhotoTemplates(photoStorage->getPhotoTemplates(), shader);
		templ->init();//photoarray		
	}
}

void PhotoTemplate::reset(PhotoboothSettingsRef settings)
{
	IPhotoboothLocation::reset(settings);

	templates = settings->getPhotoCardStylesActiveTemplate();
	stickers  = settings->getPhotoOverActiveTemplate();

	title    = settings->getTexture("printtitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), titlePositionY - 0.5f * title.getHeight());	

	Vec2f position, size;
	templatebtns.clear();

	position = Vec2f(220.0f, 420.0f);
	size = Vec2f(218.0f, 655.0f);
	templatebtns.push_back(TemplateButton1Ref(new TemplateButton1(Rectf(position, position + size), templates, stickers)));	

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	auto templ2 = TemplateButton2Ref(new TemplateButton2(Rectf(position, position + size), templates, stickers));
	templatebtns.push_back(templ2);

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 202.0f);
	//templatebtns.push_back(TemplateButton3Ref(new TemplateButton3(Rectf(position, position + size), templates, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(202.0f, 304.0f);
	//templatebtns.push_back(TemplateButton4Ref(new TemplateButton4(Rectf(position, position + size), templates, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 202.0f);
	//templatebtns.push_back(TemplateButton5Ref(new TemplateButton5(Rectf(position, position + size), templates, stickers)));
	
	for (auto templ : templatebtns)		
		templ->setSelectDesign(settings->getTexture("print"));	

	templ2->setLineTexture(settings->getTexture("printline"));
	templ2->setSelectRamkaTexture(settings->getTexture("printramka"));
}

void PhotoTemplate::stop()
{
	console()<<"PhotoTemplate stop!"<<endl;

	for (auto templ : templatebtns)
	{
		templ->disconnectEventHandler();
		templ->unActivateListeners();
	}

	selectedTemplate = nullptr;
}

void PhotoTemplate::photoTemplateChoose(EventGUIRef& event)
{
	EventGUI *ev = event.get();	
	if(!ev || typeid(*ev) != typeid(PhotoTemplateChooseEvent))
		return;

	auto eventref = static_pointer_cast<PhotoTemplateChooseEvent>(event);	
	auto id = eventref->getTemplateID();

	if(selectedTemplate)
	{
		if(selectedTemplate->getID() == id)
		{			
			setChoosingTemplate();
			nextLocationSignal();
			return;
		}
		else
			selectedTemplate->setSelected(false);
	}
	
	for (int i = 0; i < templatebtns.size(); i++)
	{
		if(templatebtns[i]->getID() == id)
		{
			selectedTemplate = templatebtns[i];
			break;
		}
	}
	
	selectedTemplate->setSelected(true);	
}

void PhotoTemplate::setChoosingTemplate()
{
	
}

void PhotoTemplate::update()
{

}

void PhotoTemplate::draw()
{	
	fillBg();
	gl::draw(title, titlePos);	

	for (auto templ : templatebtns)	
		templ->draw();	
}