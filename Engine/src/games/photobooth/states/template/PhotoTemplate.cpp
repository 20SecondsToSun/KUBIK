#include "states/template/PhotoTemplate.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::app;
using namespace ci;

PhotoTemplate::PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage) : photoStorage(photoStorage)
{
	reset(settings);
}

void PhotoTemplate::start()
{
	logger().log("~~~ Photobooth.SubLocation PhotoTemplate.Start ~~~");

	titleAlpha = 0.0f;
	for (auto &templ : templatebtns)
	{
		templ->setAlpha(0.0f);
	}		

	using namespace shaders::imagefilters;
	auto filterID = photoStorage->getSelectedFilter();
	auto shader = shadertool().get((ShaderTool::FilterType)filterID);

	for (auto templ : templatebtns)
	{
		templ->setSelected(false);
		templ->setPhotoTemplates(photoStorage->getPhotoTemplates(), shader);
		templ->init();
	}

	state = TEMPLATE_CHOOSE;
	titleAnimPosition = titlePos - Vec2f(0.0f, 170.0f);	

	delaycall(bind(&PhotoTemplate::initShowAnim, this), 0.2f);
}

void PhotoTemplate::initShowAnim()
{
	timeline().apply(&titleAlpha, 0.0f, 1.0f, animShowTitleTime + 0.2f);
	timeline().apply(&titleAnimPosition, titlePos, animShowTitleTime, EaseOutExpo());

	float delay = 0.0f, showingTime = 0.7f;
	for (auto templ : templatebtns)
	{
		templ->showAnimate(0.0f, 1.0f, 0.7f, delay);
		templ->showPositionAnimate(Vec2f(0.0f, -80.0f), 0.7f, delay);
		delay += 0.1f;
	}

	delaycall(bind(&PhotoTemplate::showAnimationComplete, this), showingTime + delay);
}

void PhotoTemplate::showAnimationComplete()
{
	for (auto templ : templatebtns)
	{
		templ->connectEventHandler(&PhotoTemplate::photoTemplateChoose, this);
		templ->activateListeners();
	}

	callback(COMPLETE_ANIM);
}

void PhotoTemplate::reset(PhotoboothSettingsRef settings)
{
	logger().log("~~~ Photobooth.SubLocation PhotoTemplate.Reset ~~~");

	IPhotoboothLocation::reset(settings);

	cards = settings->getCardsTextures();
	stickers = settings->getStickerTextures();

	title = settings->getTexture("printtitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), titlePositionY - 0.5f * title.getHeight());

	resetTemplateButtons();
}

void PhotoTemplate::stop()
{
	logger().log("~~~ Photobooth.SubLocation PhotoTemplate.Stop ~~~");
	
	stopAllTweens();
	clearDelaycall();

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
	if (!ev || typeid(*ev) != typeid(PhotoTemplateChooseEvent))
		return;

	auto eventref = static_pointer_cast<PhotoTemplateChooseEvent>(event);
	auto id = eventref->getTemplateID();

	if (selectedTemplate)
	{
		if (selectedTemplate->getID() == id)
		{
			startHideAnimation();
			return;
		}
		else
		{
			selectedTemplate->setSelected(false);
		}			
	}

	for (size_t i = 0; i < templatebtns.size(); i++)
	{
		if (templatebtns[i]->getID() == id)
		{
			selectedTemplate = templatebtns[i];
			break;
		}
	}		

	selectedTemplate->setSelected(true);
}

void PhotoTemplate::startHideAnimation()
{
	callback(BEGIN_ANIM);
	setChoosingTemplate();
	stop();	
	setLastScreenShot();
	state = ANIM_HIDE;
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.8f, EaseOutCubic())
		.finishFn(bind(&PhotoTemplate::callback, this, NEXT_LOC));
}

void PhotoTemplate::setChoosingTemplate()
{	
	ci::writeImage(getAppPath() / templateName, selectedTemplate->getPrintTemplate());
}

void PhotoTemplate::update()
{

}

void PhotoTemplate::draw()
{
	fillBg();

	switch (state)
	{
	case PhotoTemplate::TEMPLATE_CHOOSE:
		drawTitle();
		for (auto templ : templatebtns)
			templ->draw();
		break;

	case PhotoTemplate::ANIM_HIDE:
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(screenshot);
		gl::color(Color::white());
		break;
	}
}

void PhotoTemplate::resetTemplateButtons()
{
	Vec2f position, size;
	templatebtns.clear();

	position = Vec2f(220.0f, 420.0f);
	size = Vec2f(218.0f, 655.0f);
	templatebtns.push_back(TemplateButton1Ref(new TemplateButton1(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(560.0f, 420.0f);
	size = Vec2f(303.0f, 455.0f);
	auto templ2 = TemplateButton2Ref(new TemplateButton2(Rectf(position, position + size), cards, stickers));
	templatebtns.push_back(templ2);

	position = Vec2f(220.0f, 1191.0f);
	size = Vec2f(303.0f, 202.0f);
	templatebtns.push_back(TemplateButton3Ref(new TemplateButton3(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(611.0f, 1191.0f);
	size = Vec2f(202.0f, 304.0f);
	templatebtns.push_back(TemplateButton4Ref(new TemplateButton4(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(220.0f, 1461.0f);
	size = Vec2f(303.0f, 202.0f);
	templatebtns.push_back(TemplateButton5Ref(new TemplateButton5(Rectf(position, position + size), cards, stickers)));

	for (auto templ : templatebtns)
	{
		templ->setSelectDesign(settings->getTexture("print"));
	}		

	templ2->setLineTexture(settings->getTexture("printline"));
	templ2->setSelectRamkaTexture(settings->getTexture("printramka"));
}

void PhotoTemplate::stopAllTweens()
{
	alphaAnim.stop();

	for (auto templ : templatebtns)
	{
		templ->stopAllTweens();
	}		

	IPhotoboothLocation::stopAllTweens();
}
