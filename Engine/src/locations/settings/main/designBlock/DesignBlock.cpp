#include "main/designBlock/DesignBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignBlock::DesignBlock(ConfigSettingsRef configSettings, const Vec2i& position)
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
	designsLayout = DesignsLayoutRef(new DesignsLayout(configSettings, designsLayoutPos));
}

void DesignBlock::openButtonHandler(EventGUIRef& event)
{
	if (eventHandlerDic[OPEN_EVENT])
	{
		designBtn->disconnectEventHandler();
		eventHandlerDic[OPEN_EVENT]();
	}
}

void DesignBlock::activateListeners()
{
	if (designOpened)
	{
		designBtn->disconnectEventHandler();

		designsLayout->activateListeners();
		designsLayout->connectEventHandler(&DesignBlock::screenSaverStateChanged, this, DesignsLayout::SCREEN_SAVER_STATE);
		designsLayout->connectEventHandler(&DesignBlock::screenSaverOpenFolder, this, DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
		designsLayout->connectEventHandler(&DesignBlock::changedKubikDesign, this, DesignsLayout::CHANGED_DESIGN);
		designsLayout->connectEventHandler(&DesignBlock::openUserDesignFolder, this, DesignsLayout::OPEN_USER_DESIGN_FOLDER);
		designsLayout->connectEventHandler(&DesignBlock::hideHandler, this, DesignsLayout::HIDE);
	}
	else
	{
		designsLayout->unActivateListeners();
		designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_STATE);
		designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
		designsLayout->disconnectEventHandler(DesignsLayout::CHANGED_DESIGN);
		designsLayout->disconnectEventHandler(DesignsLayout::OPEN_USER_DESIGN_FOLDER);
		designsLayout->disconnectEventHandler(DesignsLayout::HIDE);

		designBtn->connectEventHandler(&DesignBlock::openButtonHandler, this);
	}
}

void DesignBlock::unActivateListeners()
{
	designsLayout->unActivateListeners();
	designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_STATE);
	designsLayout->disconnectEventHandler(DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
	designsLayout->disconnectEventHandler(DesignsLayout::CHANGED_DESIGN);
	designsLayout->disconnectEventHandler(DesignsLayout::OPEN_USER_DESIGN_FOLDER);
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

void DesignBlock::changedKubikDesign()
{
	callback(CHANGED_DESIGN);
}

void DesignBlock::openUserDesignFolder()
{
	callback(OPEN_USE_DESIGN_FOLDER);
}

void DesignBlock::hideHandler()
{
	callback(HIDE);
}

void DesignBlock::showDesigns(const EaseFn& eFunc, float time)
{
	designOpened = true;
	addChild(designsLayout);

	delayTimer = 0.0f;
	timeline().apply(&delayTimer, 1.0f, time, eFunc).finishFn(bind(&DesignBlock::animationFinish, this));
}

void DesignBlock::animationFinish()
{
	callback(OPENED);
}

void DesignBlock::hideDesigns(const EaseFn& eFunc, float time)
{
	delayTimer = 0.0f;
	timeline().apply(&delayTimer, 1.0f, time, eFunc).finishFn(bind(&DesignBlock::animationHideFinish, this));
}

void DesignBlock::animationHideFinish()
{
	designOpened = false;
	removeChild(designsLayout);

	callback(HIDED);
}

void DesignBlock::drawLayout()
{
	gl::color(iconColor);
	gl::draw(icon, Vec2i(10.0f, 48.0f));
	textTools().textFieldDraw(titleText, Vec2f(82.0f, 40.0f));
	textTools().textFieldDraw(subTitleText, Vec2f(89.0f, 100.0f));
	Sprite::drawLayout();
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

int DesignBlock::getDesignID() const
{
	return designsLayout->getDesignID();
}

void DesignBlock::setOpened(bool value)
{
	opened = value;
}

bool DesignBlock::getOpened() const
{
	return opened;
}