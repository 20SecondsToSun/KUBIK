#include "TuneUpScreen.h"

TuneUpScreen::TuneUpScreen(TuneUpSettings* config, MenuSettings* menuConfig):IScreen()
{	
	settings = config;//static_cast<TuneUpSettings*>(config);
	this->menuConfig = menuConfig;
	setTextures();
}

TuneUpScreen::~TuneUpScreen()
{	
	console()<<"~~~~~~~~~~~~~~~ TuneUpScreen destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();	
	appSettingsChgListener.disconnect();	
	designTexures.clear();
}

void TuneUpScreen::setTextures()
{
	addToDictionary("helvetica90",  settings->getFontsPath() + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 90);
	addToDictionary("helvetica20",  settings->getFontsPath() + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 20);
	addToDictionary("closeImg",		settings->getDesignPath() + "close.png",    resourceType::IMAGE, loadingType::FULL_PATH );
}

void TuneUpScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&TuneUpScreen::mouseUp, this);	
}

void TuneUpScreen::mouseUp(MouseEvent &event)
{
	closeBtn->mouseUpHandler(event.getPos());
	menuDesignChngBtn->mouseUpHandler(event.getPos());

}

void TuneUpScreen::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void TuneUpScreen::init()
{
	font =  designTexures["helvetica90"]->font;
	Font fontBtn =  designTexures["helvetica20"]->font;

	menuDesignChngBtn = new ButtonText(Rectf(150.0f, 250.0f, 350.0f, 350.0f), "Ìåíþ äèçàéí", fontBtn);	
	appSettingsChgListener = menuDesignChngBtn->mouseUpSignal.connect(bind(&TuneUpScreen::appSettingsChgHandler, this, std::placeholders::_1));

	Texture closeImg = designTexures["closeImg"]->tex;
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100, 100)));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&TuneUpScreen::closeLocationHandler, this, std::placeholders::_1));
}

void TuneUpScreen::closeLocationHandler(Button& button )
{	
	console()<<"TuneUpScreen close event::"<<endl;
	closeLocationSignal();
}

void TuneUpScreen::appSettingsChgHandler(ButtonText& button )
{
	console()<<"App settings changed event::"<<endl;
	menuConfig->setDesignPath("kubik\\templates\\2\\menuDesign\\");

	vector<SettingTypes> changes;
	changes.push_back(SettingTypes::MENU);
	appSettingsChangedSignal(changes);
}

void TuneUpScreen::draw()
{
	gl::color(Color(1,0,0));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	textTools().textFieldDraw("ÍÀÑÒÐÎÉÊÈ", &font, Vec2f(100, 100), Color::white());

	menuDesignChngBtn->draw();
	closeBtn->draw();
}