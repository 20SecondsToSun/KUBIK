#pragma once
#include "gui/Sprite.h"
#include "main/DesignBlock/DesignsLayout.h"

namespace kubik
{
	namespace config
	{
		class DesignBlock: public Sprite
		{
		public:	
			static const int OPEN_EVENT = 0;
			static const int OPENED = 1;
			static const int SCREEN_SAVER_STATE = 2;
			static const int SCREEN_SAVER_OPEN_FOLDER = 3;
			static const int CHANGED_DESIGN = 4;
			static const int OPEN_USE_DESIGN_FOLDER = 5;
			static const int HIDE = 6;		
			static const int HIDED = 7;

			DesignBlock(ConfigSettingsRef configSettings, Vec2i position)
						:Sprite(),
						titleText(configSettings->getTextItem(ConfigTextID::DESIGNMAIN)), 
						subTitleText(configSettings->getTextItem(ConfigTextID::DESIGNSUB)),				
						icon(configSettings->getTexture("designIcon")),
						iconColor(Color::white()),
						designOpened(false)
			{
				setPosition(position);

				designBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(ci::Vec2f::zero(), ci::Vec2f(880.0f, 175.0f))));	
				addChild(designBtn);

				designsLayoutPos = ci::Vec2f(89.0f, -222.0f);
				designsLayout = DesignsLayoutRef(new DesignsLayout(configSettings, designsLayoutPos));								
			}

			void openButtonHandler(EventGUIRef& event)
			{
				if(eventHandlerDic[OPEN_EVENT])
				{
					designBtn->disconnectEventHandler();
					eventHandlerDic[OPEN_EVENT]();				
				}
			}

			void activateListeners()
			{
				if(designOpened)
				{					
					designBtn->disconnectEventHandler();	

					designsLayout->activateListeners();			
					designsLayout->connectEventHandler(&DesignBlock::screenSaverStateChanged,	this, DesignsLayout::SCREEN_SAVER_STATE);
					designsLayout->connectEventHandler(&DesignBlock::screenSaverOpenFolder,		this, DesignsLayout::SCREEN_SAVER_OPEN_FOLDER);
					designsLayout->connectEventHandler(&DesignBlock::changedKubikDesign,		this, DesignsLayout::CHANGED_DESIGN);
					designsLayout->connectEventHandler(&DesignBlock::openUserDesignFolder,		this, DesignsLayout::OPEN_USER_DESIGN_FOLDER);
					designsLayout->connectEventHandler(&DesignBlock::hideHandler,				this, DesignsLayout::HIDE);
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

			void screenSaverStateChanged()
			{
				if(eventHandlerDic[SCREEN_SAVER_STATE])
					eventHandlerDic[SCREEN_SAVER_STATE]();	
			}

			void screenSaverOpenFolder()
			{
				if(eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER])
					eventHandlerDic[SCREEN_SAVER_OPEN_FOLDER]();	
			}

			void changedKubikDesign()
			{
				if(eventHandlerDic[CHANGED_DESIGN])
					eventHandlerDic[CHANGED_DESIGN]();	
			}

			void openUserDesignFolder()
			{
				if(eventHandlerDic[OPEN_USE_DESIGN_FOLDER])
					eventHandlerDic[OPEN_USE_DESIGN_FOLDER]();	
			}

			void hideHandler()
			{
				if(eventHandlerDic[HIDE])
					eventHandlerDic[HIDE]();	
			}

			void showDesigns(EaseFn eFunc, float time)
			{
				designOpened = true;
				addChild(designsLayout);	
				
				delayTimer = 0.0f;
				timeline().apply( &delayTimer, 1.0f, time, eFunc).finishFn(bind( &DesignBlock::animationFinish, this));
			}

			void animationFinish()
			{
				if(eventHandlerDic[OPENED])
					eventHandlerDic[OPENED]();					
			}

			void hideDesigns(EaseFn eFunc, float time)
			{
				designOpened = false;
				designsLayout->disconnectEventHandler();		
				removeChild(designsLayout);
			}

			virtual void drawLayout()
			{
				gl::color(iconColor);
				gl::draw(icon, ci::Vec2i(10, 48));
				textTools().textFieldDraw(titleText, ci::Vec2f(82, 40));
				textTools().textFieldDraw(subTitleText, ci::Vec2f(89, 100));
				Sprite::drawLayout();
			}	

			void setAlpha(float  alpha)
			{
				titleText.setColor(Utils::colorAlpha(titleText.getColor(), alpha));	
				subTitleText.setColor(Utils::colorAlpha(subTitleText.getColor(), alpha));	
				iconColor = Utils::colorAlpha(iconColor, alpha);	
				designBtn->setAlpha(0);
			}

			/////////////////////

			bool getScreenSaverValue()
			{
				return designsLayout->getScreenSaverValue();
			}

			int getDesignID()
			{
				return designsLayout->getDesignID();
			}

		private:
			ci::gl::Texture icon;
			ci::ColorA  iconColor;
			ci::Vec2f designsLayoutPos;
			ci::Anim<ci::Vec2f> animatePosition;
			bool designOpened;

			TextItem				titleText, subTitleText;			
			SimpleSpriteButtonRef designBtn;
			DesignsLayoutRef	  designsLayout;

			ci::Anim<float> delayTimer;
		};

		typedef std::shared_ptr<DesignBlock> DesignBlockRef;
	}
}