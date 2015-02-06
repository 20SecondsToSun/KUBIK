#pragma once
#include "gui/Dispatcher.h"
#include "main/DesignBlock/DesignsLayout.h"

namespace kubik
{
	namespace config
	{
		class DesignBlock:public CompositeDispatcher
		{
		public:	
			DesignBlock(ConfigSettingsRef configSettings, Vec2i position)
						:CompositeDispatcher(),
						titleText(configSettings->getTextItem(ConfigTextID::DESIGNMAIN)), 
						subTitleText(configSettings->getTextItem(ConfigTextID::DESIGNSUB)),				
						icon(configSettings->getTexture("designIcon")),
						iconColor(Color::white()),
						designOpened(false)
			{
				setPosition(position);
				designBtn = SimpleButtonRef(new SimpleButton(Rectf(0.0f, 0.0f, 880.0f, 175.0f)));	
				//designBtn->addMouseUpListener(&DesignBlock::mouseUpFunction, this);							
				addChild(designBtn);

				designsLayoutPos = ci::Vec2f(89.0f, 175.0f);
				designsLayout = DesignsLayoutRef(new DesignsLayout(configSettings, designsLayoutPos));	
				designsLayout->addMouseUpListener(&DesignBlock::mouseUpFunction, this);								
			}
			void activateListeners()
			{
				if(designOpened)
				{
					console()<<"opened"<<endl;
					designsLayout->activateListeners();
					//designBtn->unActivateListeners();
					designBtn->removeMouseUpListener();		
				}
				else
				{
					console()<<"close"<<endl;
					designsLayout->unActivateListeners();
				//	designBtn->activateListeners();
					designBtn->addMouseUpListener(&DesignBlock::mouseUpFunction, this);			
				}
			}

			void showDesigns(EaseFn eFunc, float time)
			{
				designOpened = true;
				addChild(designsLayout);	
			}

			void hideDesigns(EaseFn eFunc, float time)
			{
				designOpened = false;
				//designsLayout->removeMouseUpListener();		
				removeChild(designsLayout);
			}

			virtual void drawLayout()
			{
				gl::color(iconColor);
				gl::draw(icon, ci::Vec2i(10, 48));
				textTools().textFieldDraw(titleText, Vec2f(82, 40));
				textTools().textFieldDraw(subTitleText, Vec2f(89, 100));
				CompositeDispatcher::drawLayout();
			}	

			void setAlpha(float  alpha)
			{
				titleText.setColor(Utils::colorAlpha(titleText.getColor(), alpha));	
				subTitleText.setColor(Utils::colorAlpha(subTitleText.getColor(), alpha));	
				iconColor = Utils::colorAlpha(iconColor, alpha);	
				designBtn->setAlpha(0);
			}

			void setIcon(Texture tex)
			{
				icon = tex;
			}				

		private:
			ci::gl::Texture icon;
			TextItem titleText, subTitleText;
			ci::ColorA  iconColor;
			SimpleButtonRef designBtn;
			DesignsLayoutRef designsLayout;

			ci::Vec2f designsLayoutPos;
			ci::Anim<ci::Vec2f> animatePosition;

			bool designOpened;
		};

		typedef std::shared_ptr<DesignBlock> DesignBlockRef;
	}
}