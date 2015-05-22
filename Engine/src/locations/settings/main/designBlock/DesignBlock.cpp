#include "main/designBlock/DesignBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignBlock::DesignBlock(ConfigSettingsRef configSettings, ScreenSaverSettingsRef ssSettings, const Vec2i& position)
	:Sprite(),
	titleText(configSettings->getTextItem(ConfigTextID::DESIGNMAIN)),
	subTitleText(configSettings->getTextItem(ConfigTextID::DESIGNSUB)),
	icon(configSettings->getTexture("designIcon")),
	iconColor(Color::white()),
	designOpened(false),
	opened(false)
{
	setPosition(position);

	designBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(Vec2f::zero(), Vec2f(880.0f, 175.0f))));
	addChild(designBtn);

	designsLayoutPos = Vec2f(89.0f, 222.0f);
	designsLayout = DesignsLayoutRef(new DesignsLayout(configSettings, ssSettings, designsLayoutPos));
}

void DesignBlock::openButtonHandler(EventGUIRef& event)
{
	opened ? callback(CLOSE_EVENT) : callback(OPEN_EVENT);
}

void DesignBlock::callbackHandler(EventGUIRef& event)
{	
	EventGUI *ev = event.get();	
	if (ev) mouseUpSignal(event);		
}

void DesignBlock::activateListeners()
{
	if (designOpened)
	{
		//designBtn->disconnectEventHandler();
		designsLayout->activateListeners();
		designsLayout->connectEventHandler(&DesignBlock::callbackHandler, this);


		designsLayout->connectEventHandler(&DesignBlock::screenSaverStateChanged, this, DesignsLayout::SCREEN_SAVER_STATE);
		designsLayout->connectEventHandler(&DesignBlock::screenSaverOpenFolder, this, DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
		designsLayout->connectEventHandler(&DesignBlock::hideHandler, this, DesignsLayout::HIDE);
	}
	else
	{
		designsLayout->unActivateListeners();
		designsLayout->disconnectEventHandler();


		designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_STATE);
		designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
		designsLayout->disconnectEventHandler(DesignsLayout::HIDE);

		designBtn->connectEventHandler(&DesignBlock::openButtonHandler, this);
	}
}

void DesignBlock::unActivateListeners()
{
	designsLayout->unActivateListeners();
	designsLayout->disconnectEventHandler();

	designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_STATE);
	designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
	designsLayout->disconnectEventHandler(DesignsLayout::HIDE);

	designBtn->disconnectEventHandler();
}

void DesignBlock::screenSaverStateChanged()
{
	callback(SCREEN_SAVER_STATE);
}

void DesignBlock::screenSaverOpenFolder()
{
	callback(SCREEN_SAVER_OPEN_FOLDER);
}

void DesignBlock::hideHandler()
{
	callback(HIDE);
}

void DesignBlock::showDesigns(const EaseFn& eFunc, float time)
{
	designOpened = true;
	addChildFront(designsLayout);

	timeline().apply(&animatePosition, designsLayoutPos - Vec2f(0.0f, 1126.0f), designsLayoutPos, time, eFunc)
		.updateFn(bind(&DesignBlock::posAnimationUpdate, this))
		.finishFn(bind(&DesignBlock::animationFinish, this));
}

void DesignBlock::posAnimationUpdate()
{
	designsLayout->setPosition(animatePosition.value());
}

void DesignBlock::animationFinish()
{
	callback(OPENED);
}

void DesignBlock::hideDesigns(const EaseFn& eFunc, float time)
{
	timeline().apply(&animatePosition, designsLayoutPos, designsLayoutPos - Vec2f(0.0f, 1126.0f), time, eFunc)
		.updateFn(bind(&DesignBlock::posAnimationUpdate, this))
		.finishFn(bind(&DesignBlock::animationHideFinish, this));	
}

void DesignBlock::animationHideFinish()
{
	designOpened = false;
	removeChild(designsLayout);

	callback(HIDED);
}

void DesignBlock::draw()
{	
	for (auto comp : displayList)
		comp->draw();

	gl::pushMatrices();				
	gl::translate(getGlobalPosition());
	drawLayout();
	gl::popMatrices();	
}

void DesignBlock::drawLayout()
{
	//Sprite::drawLayout();

	gl::color(Color::hex(0x0d0917));
	gl::drawSolidRect(designBtn->getButtonArea());
	gl::color(iconColor);

	gl::pushMatrices();	
	gl::translate(Vec2i(10.0f, 48.0f));
	if(designOpened)
	{
		gl::translate(25.0f, -10.0f);
		gl::rotate(45.0f);
	}
	
	gl::draw(icon);
	gl::popMatrices();
	
	textTools().textFieldDraw(titleText,	Vec2f(82.0f, 40.0f));
	textTools().textFieldDraw(subTitleText, Vec2f(89.0f, 100.0f));	
}

void DesignBlock::setAlpha(float alpha)
{
	titleText.setColor(Utils::colorAlpha(titleText.getColor(), alpha));
	subTitleText.setColor(Utils::colorAlpha(subTitleText.getColor(), alpha));
	iconColor = Utils::colorAlpha(iconColor, alpha);
	designBtn->setAlpha(0.0f);
}

/////////////////////

bool DesignBlock::getScreenSaverValue() const
{
	return designsLayout->getScreenSaverValue();
}

void DesignBlock::setOpened(bool value)
{
	opened = value;
}

bool DesignBlock::getOpened() const
{
	return opened;
}