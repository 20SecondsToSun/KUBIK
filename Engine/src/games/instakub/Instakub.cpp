#include "Instakub.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

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
	hashtagOnly = HashtagOnlyRef(new HashtagOnly(settings));
	searchOnly = SearchOnlyRef(new SearchOnly(settings));
	hashTagAndSearch = HashtagAndSearchRef(new HashtagAndSearch(settings));

	view = hashtagOnly;
}

Instakub::~Instakub()
{
	console()<<"~~~~~~~~~~~~~~~~~~~~~~~Instakub destructor~~~~~~~~~~~~~~~~~~~"<<endl;	
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

	view->start();

	initShowAnimation();
}

void Instakub::showAnimationComplete()
{
	callback(ENABLE_GAME_CLOSE);
	state = DRAW;
}

void Instakub::stop()
{
	console() << "STOP Instakub!!!" << endl;
}

void Instakub::reset()
{
	hashtagOnly->reset();
	searchOnly->reset();
	hashTagAndSearch->reset();
}

void Instakub::closeMouseUpHandler(IButton& button )
{	
	closeLocationSignal();
}

void Instakub::draw()
{
	switch (state)
	{
	case SHOW_ANIM:
		screenshotDraw();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		//gl::color(Color(1, 1, 0));
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
		view->draw();
		gl::popMatrices();
		break;

	case DRAW:
		view->draw();
		break;
	}
}