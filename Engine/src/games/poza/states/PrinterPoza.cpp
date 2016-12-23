#include "PrinterPoza.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace kubik;
using namespace kubik::games::poza;
using namespace kubik::config;

PrinterPoza::PrinterPoza(config::PozaSettingsRef settings, kubik::games::photobooth::PhotoStorageRef photoStorage, std::vector<int>& gameScore)
	:photoStorage(photoStorage),
	animTime(0.8f),
	alphaAnim(1.0f),
	gameScore(gameScore)
{
	titlePositionY = 237.0f;
	reset(settings);
}

PrinterPoza::~PrinterPoza()
{
	logger().log("~~~ Poza.SubLocation Printer.Destruct ~~~");
}

void PrinterPoza::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation Printer.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;

	totalfail	  = settings->getTexture("totalfail");
	printerbg	  = settings->getTexture("printerbg");

	printBtn	  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("printbtn")));
	againBtn	  = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("againbtn")));
	othergamesBtn = ImageButtonSpriteRef(new ImageButtonSprite(settings->getTexture("othergamesbtn")));

	buttonY = 1705.0f; 	

	auto pozasettings   = static_pointer_cast<PozaSettings>(set);
	currentCardTemplate = pozasettings->getCurrentTemplate();
}

void PrinterPoza::start()
{
	logger().log("~~~ Poza.SubLocation Printer.Start ~~~");

	state = PrinterState::FAIL;

	for (size_t i = 0; i < gameScore.size(); i++)
	{
		if (gameScore[i])
		{
			state = PrinterState::PASS;
			break;
		}
	}	

	if (state == PrinterState::PASS)
	{
		buttonY = 1705.0f;
	}
	else
	{
		buttonY = 1785.0f;
	}

	againBtn->setPosition(Vec2f(550.0f, buttonY - againBtn->getHeight() * 0.5f));
	printBtn->setPosition(Vec2f(155.0f, buttonY - printBtn->getHeight() * 0.5f));
	othergamesBtn->setPosition(Vec2f(155.0f, buttonY - printBtn->getHeight() * 0.5f));

	photoItems = photoStorage->getPhotoItems();

	delaycall(bind(&PrinterPoza::initAnimationcomplete, this), 0.4f);
}

void PrinterPoza::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);

	againBtn->connectEventHandler(&PrinterPoza::againBtnHandler, this);

	if (state == PrinterState::PASS)
	{		
		printBtn->connectEventHandler(&PrinterPoza::printBtnHandler, this);
	}
	else
	{
		othergamesBtn->connectEventHandler(&PrinterPoza::othergamesBtnHandler, this);
	}
}

void PrinterPoza::againBtnHandler(EventGUIRef& event)
{
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&PrinterPoza::hideAnimationComplete, this));

	callback(BEGIN_ANIM);	
}

void PrinterPoza::printBtnHandler(EventGUIRef& event)
{
	nextLocationSignal();
}

void PrinterPoza::othergamesBtnHandler(EventGUIRef& event)
{
	callback(CLOSE_LOCATION);
}

void PrinterPoza::stop()
{
	logger().log("~~~ Poza.SubLocation Social.Stop ~~~");

	stopAllTweens();
	disconnectEventHandlers();
}

void PrinterPoza::hideAnimation(EventGUIRef& event)
{
	//voidBtn->disconnectEventHandler();
	//callback(BEGIN_ANIM);
	//app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&Printer::hideAnimationComplete, this));
}

void PrinterPoza::hideAnimationComplete()
{
	stop();
	alphaAnim = 1.0f;	
	callback(FIRST_LOC);
}

void PrinterPoza::update()
{

}

void PrinterPoza::draw()
{
	fillBg();

	if (state == PrinterState::PASS)
	{
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(printerbg);
		againBtn->setAlpha(alphaAnim);
		printBtn->setAlpha(alphaAnim);
		againBtn->draw();
		printBtn->draw();

		vector<Vec2f> positions;
		positions.push_back(Vec2f(195, 380));
		positions.push_back(Vec2f(575, 380));
		positions.push_back(Vec2f(385, 925));
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		int index = 0;
		for (size_t i = 0; i < gameScore.size(); i++)
		{
			if (gameScore[i])
			{
				auto img = photoStorage->getChoosingPreview()[index++];

				if (currentCardTemplate)
				{
					gl::pushMatrices();
					gl::translate(positions[i]);
					gl::scale(310.0f / currentCardTemplate.getWidth(), 310.0f / currentCardTemplate.getWidth());
					gl::draw(currentCardTemplate);
					gl::popMatrices();
				}

				if (img)
				{
					gl::pushMatrices();
					gl::translate(positions[i]);
					gl::scale(310.0f / img.getWidth(), 310.0f / img.getWidth());
					gl::draw(img);
					gl::popMatrices();
				}				
			}
		}
	}
	else
	{
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
		gl::draw(totalfail);
		againBtn->setAlpha(alphaAnim);
		othergamesBtn->setAlpha(alphaAnim);
		againBtn->draw();
		othergamesBtn->draw();
	}
}

void PrinterPoza::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}

void PrinterPoza::disconnectEventHandlers()
{
	againBtn->disconnectEventHandler();

	if (state == PrinterState::PASS)
	{
		printBtn->disconnectEventHandler();
	}
	else
	{
		othergamesBtn->disconnectEventHandler();
	}
}