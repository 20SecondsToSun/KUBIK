#include "Instakub.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

const float Instakub::RELOAD_TIME = 20.0f;

Instakub::Instakub(ISettingsRef config)
{	
	console()<<":: Instakub CREATED::"<<endl;
	init(config);
	setType(ScreenId::INSTAKUB);
	create();
}

void Instakub::init(ISettingsRef config)
{
	settings = static_pointer_cast<InstakubSettings>(config);
}

void Instakub::create()
{
	hashtagOnly		 = HashtagOnlyRef(new HashtagOnly(settings));
	searchOnly		 = SearchOnlyRef(new SearchOnly(settings));
	hashTagAndSearch = HashtagAndSearchRef(new HashtagAndSearch(settings));

	view = hashtagOnly;
}

Instakub::~Instakub()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Instakub destructor~~~~~~~~~~~~~~~~~~~~~~~"<<endl;	
}

void Instakub::start()
{
	console() << "START Instakub!!!" << endl;

	if (!settings->hashtagEnabled())
	{
		view = searchOnly;
		console() << "search only" << endl;
	}		
	else if (!settings->searchEnabled())
	{
		view = hashtagOnly;
		console() << "hashtagOnly" << endl;
	}		
	else
	{
		view = hashTagAndSearch;
		console() << "hashTagAndSearch" << endl;
	}

	view->connectEventHandler(&Instakub::enableControls, this, InstakubLocation::ENABLE_CONTROLS);
	view->connectEventHandler(&Instakub::disableControls, this, InstakubLocation::DISABLE_CONTROLS);
	view->connectEventHandler(&Instakub::showControls, this, InstakubLocation::SHOW_CONTROLS);
	view->connectEventHandler(&Instakub::hideControls, this, InstakubLocation::HIDE_CONTROLS);	
	view->start();

	initShowAnimation();
	changeState(SHOW_ANIM);	
}

void Instakub::showAnimationComplete()
{	
	changeState(DRAW);
	
	view->load();
	connectEventHandler(&Instakub::mouseTouched, this);
	startTimer();
}

void Instakub::mouseTouched(EventGUIRef& event)
{
	resetTimer();
}

void Instakub::stop()
{
	console() << "STOP Instakub!!!" << endl;
	view->disconnectEventHandler(InstakubLocation::ENABLE_CONTROLS);
	view->disconnectEventHandler(InstakubLocation::DISABLE_CONTROLS);
	view->disconnectEventHandler(InstakubLocation::SHOW_CONTROLS);
	view->disconnectEventHandler(InstakubLocation::HIDE_CONTROLS);
	view->stop();
	disconnectEventHandler();
	stopTimer();	
}

void Instakub::startTimer()
{
	delaycall(bind(&Instakub::timerComplete, this), 20.0);
}

void Instakub::resetTimer()
{
	console()<<"----reset timer----"<<endl;
	stopTimer();
	startTimer();
}

void Instakub::stopTimer()
{
	clearDelaycall();
}

void Instakub::timerComplete()
{
	console()<<"----timerComplete----"<<endl;
	view->reload();
	resetTimer();
}

void Instakub::reset()
{
	hashtagOnly->reset();
	searchOnly->reset();
	hashTagAndSearch->reset();
}

void Instakub::draw()
{
	(this->*drawFuncPointer)();	
}

void Instakub::animDrawings()
{
	screenshotDraw();

	gl::pushMatrices();
	gl::translate(animX, 0.0f);
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	view->draw();
	gl::popMatrices();
}

void Instakub::viewDrawings()
{
	view->draw();
}

void Instakub::closeMouseUpHandler(IButton& button)
{
	closeLocationSignal();
}

void Instakub::enableControls()
{
	callback(ENABLE_GAME_CLOSE);
}

void Instakub::disableControls()
{
	callback(DISABLE_GAME_CLOSE);
}

void Instakub::showControls()
{
	callback(SHOW_CONTROLS);
}

void Instakub::hideControls()
{
	callback(HIDE_CONTROLS);
}

void Instakub::changeState(const locationStates& state)
{
	this->state = state;

	switch (state)
	{
	case DRAW:
		drawFuncPointer = &Instakub::viewDrawings;
		break;

	case SHOW_ANIM:
		drawFuncPointer = &Instakub::animDrawings;
		break;
	}
}