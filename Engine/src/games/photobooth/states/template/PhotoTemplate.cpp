#include "states/template/PhotoTemplate.h"
#include "printer/Printer.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::photobooth;
using namespace std;
using namespace ci::app;
using namespace ci;

PhotoTemplate::PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage) 
	:photoStorage(photoStorage)
{
	reset(settings);
}

void PhotoTemplate::start()
{
	logger().log("~~~ Photobooth.SubLocation PhotoTemplate.Start ~~~");
		
	using namespace shaders::imagefilters;
	auto filterID = photoStorage->getSelectedFilter();
	auto shader = shadertool().get((ShaderTool::FilterType)filterID);

	for (auto templ : templatebtns)
	{
		templ->setSelected(false);
		templ->setPhotoTemplates(photoStorage->getPhotoTemplates(), shader);
		templ->init();
	}

	state = INIT;
	titleAnimPosition = titlePos - Vec2f(0.0f, 170.0f);		 
	titleAlpha = 0.0f;

	for (auto &templ : templatebtns)
	{
		templ->setAlpha(0.0f);
	}

	delaycall(bind(&PhotoTemplate::initShowAnim, this), 0.4f);
}

void PhotoTemplate::initShowAnim()
{
	state = TEMPLATE_CHOOSE;

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

void PhotoTemplate::reset(ISettingsRef settings)
{
	logger().log("~~~ Photobooth.SubLocation PhotoTemplate.Reset ~~~");

	IGameLocation::reset(settings);
	auto setphoto = static_pointer_cast<PhotoboothSettings>(settings);

	cards    = setphoto->getPhotoCardsTextures();
	stickers = setphoto->getStickerTextures();

	title	 = settings->getTexture("printtitle");
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
	auto setphoto = static_pointer_cast<PhotoboothSettings>(settings);

	if (selectedTemplate)
	{
		if (selectedTemplate->getID() == id)
		{
			startHideAnimation();
			setphoto->addPrintedCount();
			dbRecord->PrintCardTemplate = id;
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
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.8f, EaseOutCubic()).finishFn(bind(&PhotoTemplate::callback, this, NEXT_LOC));
}

void PhotoTemplate::setChoosingTemplate()
{		
	gl::Texture printTemplate;
	switch (selectedTemplate->getID())
	{
		case FIRST:
		{			
			printTemplate = Utils::drawGraphicsToFBO(printer().getPixelsSize(), [&]()
			{
				auto templateScale = 0.5f * printer().getPixelsSize().x / selectedTemplate->getPrintTemplate().getWidth();
				gl::scale(templateScale, templateScale);
				gl::draw(selectedTemplate->getPrintTemplate());
				gl::translate(selectedTemplate->getPrintTemplate().getWidth() - 5, 0);// FOR PRINTER PIXELS SHIFT, SHIT BUT THIS NESSESSERY
				gl::draw(selectedTemplate->getPrintTemplate());
			});
			printer().applyPhotoBoothSettings(DMPAPER_BOOTH_TWO_PHOTOS);
		}		
		break;

		case SECOND:
		{
			printTemplate = Utils::drawGraphicsToFBO(printer().getPixelsSize(), [&]()
			{ 
				auto templateScale = printer().getPixelsSize().x / selectedTemplate->getPrintTemplate().getWidth();
				gl::scale(templateScale, templateScale);
				gl::draw(selectedTemplate->getPrintTemplate());
			});
			printer().applyPhotoBoothSettings(DMPAPER_BOOTH_ONE_PHOTO);
		}
		break;

		case THIRD:
		{
			printTemplate = Utils::drawGraphicsToFBO(printer().getPixelsSize(), [&]()
			{
				auto templateScale = printer().getPixelsSize().x / selectedTemplate->getPrintTemplate().getWidth();
				
				gl::translate(selectedTemplate->getPrintTemplate().getHeight(), 0);
				gl::rotate(90);
				gl::draw(selectedTemplate->getPrintTemplate());
			});
			printer().applyPhotoBoothSettings(DMPAPER_BOOTH_ONE_PHOTO);
		}
		break;

		case FOURTH:
		{
			printTemplate = Utils::drawGraphicsToFBO(printer().getPixelsSize(), [&]()
			{
				auto templateScale = printer().getPixelsSize().x / selectedTemplate->getPrintTemplate().getWidth();
				gl::scale(templateScale, templateScale);
				gl::draw(selectedTemplate->getPrintTemplate());
			});
			printer().applyPhotoBoothSettings(DMPAPER_BOOTH_ONE_PHOTO);
		}
		break;

		case FIFTH:
		{
			printTemplate = Utils::drawGraphicsToFBO(printer().getPixelsSize(), [&]()
			{
				auto templateScale = printer().getPixelsSize().x / selectedTemplate->getPrintTemplate().getWidth();
				//gl::scale(templateScale, templateScale);
				gl::translate(selectedTemplate->getPrintTemplate().getHeight(), 0);
				gl::rotate(90);
				gl::draw(selectedTemplate->getPrintTemplate());
			});
			printer().applyPhotoBoothSettings(DMPAPER_BOOTH_ONE_PHOTO);
		}
		break;		
	}
	Surface savePhotocard(printTemplate);
	ci::writeImage(Paths::getPhotoTemplatePath(), savePhotocard);

	auto templ = photoStorage->getPhotoTemplates();

	try
	{
		for (size_t i = 0; i < 3; i++)
		{
			ci::writeImage(Paths::getPhotoTemplateToServerPath(i), templ[i][FORMAT2_PRINT]);
		}
	}
	catch (...)
	{

	}

	try
	{
		auto _width    = templ[0][FORMAT2_PRINT].getWidth();
		auto _height   = templ[0][FORMAT2_PRINT].getHeight();
		auto _heightX3 = _height * 3;

		auto tex1 = templ[0][FORMAT2_PRINT];
		auto tex2 = templ[1][FORMAT2_PRINT];
		auto tex3 = templ[2][FORMAT2_PRINT];

		auto templ = Utils::drawGraphicsToFBO(Vec2f(_width, _heightX3), [&]()
		{		
			gl::draw(tex1);
			gl::translate(0, _height);
			gl::draw(tex2);
			gl::translate(0, _height);
			gl::draw(tex3);
		});

		ci::writeImage(Paths::getPhotoTemplateRibbonToServerPath(), templ);
	}
	catch (...)
	{

	}

	//templ[2][FormatID::FORMAT1_PRINT]

	printer().print();

	logger().log("width  :: " + toString(selectedTemplate->getPrintTemplate().getWidth()));
	logger().log("height  :: " + toString(selectedTemplate->getPrintTemplate().getHeight()));
	logger().log("selectedTemplate->getID()  :: " + toString(selectedTemplate->getID()));	
}

void PhotoTemplate::update()
{

}

void PhotoTemplate::draw()
{
	fillBg();

//	gl::draw(tempImage);

	switch (state)
	{
	case PhotoTemplate::TEMPLATE_CHOOSE:
		drawTitle();
		for (auto templ : templatebtns)
		{
			templ->draw();
		}
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

	Vec2f YTemp(0, 0);

	position = Vec2f(220.0f, 420.0f) - YTemp;
	size = Vec2f(218.0f, 670.0f);
	templatebtns.push_back(TemplateButton1Ref(new TemplateButton1(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(560.0f, 420.0f) - YTemp;
	size = Vec2f(303.0f, 465.0f);
	auto templ2 = TemplateButton2Ref(new TemplateButton2(Rectf(position, position + size), cards, stickers));
	templatebtns.push_back(templ2);

	position = Vec2f(220.0f, 1191.0f) - YTemp;
	size = Vec2f(303.0f, 198.0f);
	templatebtns.push_back(TemplateButton3Ref(new TemplateButton3(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(611.0f, 1191.0f) - YTemp;
	size = Vec2f(202.0f, 315.0f);
	templatebtns.push_back(TemplateButton4Ref(new TemplateButton4(Rectf(position, position + size), cards, stickers)));

	position = Vec2f(220.0f, 1461.0f) - YTemp;
	size = Vec2f(303.0f, 198.0f);
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

	IGameLocation::stopAllTweens();
}
