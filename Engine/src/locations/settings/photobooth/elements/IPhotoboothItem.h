#pragma once
#include "gui/Sprite.h"
#include "PhotoboothSettings.h"
#include "photobooth/elements/MainTitleButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class IPhotoboothItem> IPhotoboothItemRef;

		class IPhotoboothItem: public Sprite
		{
		public:
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, PhtTextID id, ci::Color color, int index)
				:Sprite(),
				settings(phbSettings),
				state(CLOSE),
				index(index),
				itemWidth(1080 - 166),
				closeHeightMax(384),
				closeHeightMin(199),
				openHeight(1124),
				color(color),
				closeMinY(50),
				closeMaxY(130),
				openY(135),
				offsetBetweenTitles(60),
				id(id)
			{
				mainTitleY = closeMaxY;
				subTitleY  = closeMaxY + offsetBetweenTitles;
				animHeight = closeHeightMax;
				animatePosition = Vec2f(0.0f, closeHeightMax * index);

				setPosition(animatePosition.value());

				mainTitleButton = MainTitleButtonRef(new MainTitleButton(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMax)), index));				
				addChild(mainTitleButton);				
			
				setMainTextTitle(textTools().getTextField(phbSettings->getMainTitle(id)));
				setSubTextTitle(textTools().getTextField(phbSettings->getSubTitleClose(id), true));				
			}

			void setSubTextTitle(ci::gl::Texture tex)
			{
				subText			= tex;
				subTextTexPosX  = 0.5f * (itemWidth - subText.getWidth());
			}

			void setMainTextTitle(ci::gl::Texture tex)
			{
				mainTextTex		= tex;
				mainTextTexPosX	= 0.5f * (itemWidth - mainTextTex.getWidth());
			}

			virtual void activateListeners()
			{
				mainTitleButton->connectEventHandler(&IPhotoboothItem::mainTitleClicked, this);	
				Sprite::activateListeners();
			}

			virtual void mainTitleClicked(EventGUIRef& event)
			{
	
			}

			virtual void unActivateListeners()
			{
				mainTitleButton->disconnectEventHandler();
				Sprite::unActivateListeners();
			}

			virtual void draw()
			{	
				gl::pushMatrices();
					gl::translate(getGlobalPosition());
					gl::color(color);
					gl::drawSolidRect(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, animHeight)));
					gl::color(ci::Color::white());
					gl::draw(mainTextTex, Vec2f(mainTextTexPosX, mainTitleY));
					gl::draw(subText, Vec2f(subTextTexPosX, subTitleY));			
				gl::popMatrices();	

				if (state == OPEN)	
					Sprite::draw();
			}

			virtual void drawContent()
			{
			}

			int getIndex()
			{
				return index;
			}

			void setOpenLayoutIndex(int openLayoutIndex)
			{
				this->openLayoutIndex = openLayoutIndex;
			
				float time = 0.7f, height = 0.0f;
				ci::EaseFn eFunc = ci::EaseOutExpo();
				ci::Vec2f animateTo;			

				if(openLayoutIndex == index)
				{						
					openingLayout(eFunc, time);
					animateTo = ci::Vec2f(0.0f, openLayoutIndex * closeHeightMin);						
				}
				else if (openLayoutIndex == -1)
				{
					closingLayoutMaxState(eFunc, time);						
					animateTo = ci::Vec2f(0.0f, index * closeHeightMax);								
				}	
				else
				{
					closingLayoutMinState(eFunc, time);		
					if (openLayoutIndex > index)					
						animateTo =  ci::Vec2f(0.0f, index * closeHeightMin);					
					else
						animateTo =  ci::Vec2f(0.0f, (index - 1) * (closeHeightMin) + openHeight);														
				}

				timeline().apply( &animatePosition, animateTo, time, eFunc)
					.updateFn(bind( &IPhotoboothItem::posAnimationUpdate, this))
						.finishFn( bind( &IPhotoboothItem::animationFinish2, this));
			}

			void openingLayout(ci::EaseFn eFunc, float time)
			{
				state = OPEN;	
				setSubTextTitle(textTools().getTextField(settings->getSubTitleOpen(id), true));		
				onOpenResetParams();
				timeline().apply( &mainTitleY, openY, time, eFunc);
				timeline().apply( &subTitleY,  openY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, openHeight + 2, time, eFunc);	
			}

			void closingLayoutMaxState(ci::EaseFn eFunc, float time)
			{
				state = CLOSE;	
				setSubTextTitle(textTools().getTextField(settings->getSubTitleClose(id), true));		
				mainTitleButton->setButtonArea( ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMax)));
				timeline().apply( &mainTitleY, closeMaxY, time, eFunc);
				timeline().apply( &subTitleY,  closeMaxY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, closeHeightMax + 2, time, eFunc);			
			}

			void closingLayoutMinState(ci::EaseFn eFunc, float time)
			{
				state = CLOSE_MIN;	
				setSubTextTitle(textTools().getTextField(settings->getSubTitleClose(id), true));		
				mainTitleButton->setButtonArea(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMin)));
				timeline().apply( &mainTitleY, closeMinY, time, eFunc);
				timeline().apply( &subTitleY,  closeMinY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, closeHeightMin + 2, time, eFunc);
			}

			virtual void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

			void animationFinish2()
			{				
				if(state == OPEN)
					mainTitleButton->disconnectEventHandler();
				else
					mainTitleButton->connectEventHandler(&IPhotoboothItem::mainTitleClicked, this);	
			}

			virtual void onOpenResetParams()
			{

			}

		protected:
			enum {CLOSE, OPEN, CLOSE_MIN} state;
			ci::Color color;
	
			ci::Font mainTextFont, subTextFont;
			int itemWidth, index, openLayoutIndex;
			int closeHeightMin, closeHeightMax, openHeight;
			
			ci::gl::Texture mainTextTex, subText;			

			ci::Anim<ci::Vec2f> animatePosition;
			ci::Anim<float> mainTitleY, subTitleY;
			ci::Anim<int> animHeight;

			float subTextTexPosX, mainTextTexPosX;

			float closeMaxY, closeMinY, openY;
			float offsetBetweenTitles;

			PhotoboothSettingsRef settings;
			MainTitleButtonRef mainTitleButton;	
			PhtTextID id;
		};		
	}
}