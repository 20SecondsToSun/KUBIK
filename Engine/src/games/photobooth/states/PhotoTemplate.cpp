#include "states/PhotoTemplate.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games;
using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci;

PhotoTemplate::PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage):photoStorage(photoStorage)
{		
	reset(settings);			
}

void PhotoTemplate::start()
{
	console()<<"start PhotoTemplate--------------------------"<<endl;

	for (auto templ : templatebtns)	
	{
		templ->connectEventHandler(&PhotoTemplate::photoTemplateChoose, this);		
		templ->activateListeners();
		templ->setSelected(false);
		templ->init();//photoarray
	}
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
	
	selectedTemplate = templatebtns[id];
	selectedTemplate->setSelected(true);	
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

void PhotoTemplate::setChoosingTemplate()
{
	
}

void PhotoTemplate::reset(PhotoboothSettingsRef settings)
{
	templates = settings->getPhotoCardStylesActiveTemplate();
	stickers  = settings->getPhotoOverActiveTemplate();

	auto title1   = settings->getTextItem(PhtTextID::TEMPLATE_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::TEMPLATE_TEXT2);

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);

	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), 172.0f);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), 258.0f);
	
	Vec2f position, size;
	templatebtns.clear();

	position = Vec2f(220.0f, 420.0f);
	size = Vec2f(218.0f, 655.0f);
	templatebtns.push_back(TemplateButton1Ref(new TemplateButton1(Rectf(position, position + size), templates, stickers)));	

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	templatebtns.push_back(TemplateButton2Ref(new TemplateButton2(Rectf(position, position + size), templates, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	//templatebtns.push_back(TemplateButton3Ref(new TemplateButton3(Rectf(position, position + size), templates, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	//templatebtns.push_back(TemplateButton4Ref(new TemplateButton4(Rectf(position, position + size), templates, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	//templatebtns.push_back(TemplateButton5Ref(new TemplateButton5(Rectf(position, position + size), templates, stickers)));
	
	for (auto templ : templatebtns)	
	{
		auto title = settings->getTextItem(PhtTextID::TEMPLATE_PRINT);		
		templ->setSelectDesign(settings->getTexture("print"), textTools().getTextField(title));
	}
}

void PhotoTemplate::update()
{

}

void PhotoTemplate::draw()
{	
	gl::draw(tex1, title1Pos);
	gl::draw(tex2, title2Pos);

	for (auto templ : templatebtns)	
		templ->draw();	
}