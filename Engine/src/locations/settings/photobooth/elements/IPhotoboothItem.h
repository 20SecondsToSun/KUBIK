#pragma once
#include "gui/CompositeDispatcher.h"

#include "PhotoboothSettings.h"
#include "photobooth/elements/MainTitleButton.h"
#include "SaveBtn.h"

namespace kubik
{
	namespace config
	{
		class IPhotoboothItem: public CompositeDispatcher
		{
		public:
			//typedef PhotoboothSettings::SettingsPartID SettingsPartID;
			//typedef PhotoboothSettings::TextID TextID;
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, PhtTextID id, ci::Color color, int index)
				:CompositeDispatcher(),
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
				openY(170),
				offsetBetweenTitles(60)
			{
				mainTitleY = closeMaxY;
				subTitleY  = closeMaxY + offsetBetweenTitles;
				animHeight = closeHeightMax;
				animatePosition = Vec2f(0.0f, closeHeightMax * index);
				setPosition(animatePosition.value());

				mainTitleButton = MainTitleButtonRef(new MainTitleButton(Rectf(0.0f, 0.0f, itemWidth, closeHeightMax), index));				
				addChild(mainTitleButton);
				
			
				mainTextTex		= textTools().getTextField(phbSettings->getMainTitle(id));
				subTextTexClose	= textTools().getTextField(phbSettings->getSubTitleClose(id));
				subTextTexOpen	= textTools().getTextField(phbSettings->getSubTitleOpen(id));
	

				////saveBtn = SaveBtnRef(new SaveBtn(Rectf(0.0f, 0.0f, 245.0f, 65.0f), phbSettings->getText(PhtTextID::SAVE_TEXT))),	

				//saveBtn->setPosition(Vec2f(0.5 * (itemWidth - 245.0f),  835.0f));
				//addChild(saveBtn);
			}

			virtual void activateListeners()
			{
				mainTitleButton->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);	
				CompositeDispatcher::activateListeners();
			}

			virtual void unActivateListeners()
			{
				mainTitleButton->removeMouseUpListener();
				//saveBtn->removeMouseUpListener();
				//CompositeDispatcher::unActivateListeners();
			}

			virtual void mouseUpFunction(EventGUIRef event )
			{				
				EventGUI *ev = event.get();
				if(typeid(*ev) == typeid(SavePhotobootnConfigEvent))
					saveConfiguration();						
				
				mouseUpSignal(event);
			}

			virtual void saveConfiguration(){};

			virtual void draw()
			{	
				gl::pushMatrices();
				gl::translate(getGlobalPosition());
				gl::color(color);
				gl::drawSolidRect(Rectf(0, 0, itemWidth, animHeight));
				gl::color(Color::white());
				gl::draw(mainTextTex, Vec2f(0.5 * (itemWidth - mainTextTex.getWidth()), mainTitleY));
				if (state == CLOSE || state == CLOSE_MIN)
				{								
					gl::draw(subTextTexClose, Vec2f(0.5  * (itemWidth - subTextTexClose.getWidth()), subTitleY));
				}
				else
					gl::draw(subTextTexOpen, Vec2f(0.5  * (itemWidth - subTextTexOpen.getWidth()), subTitleY));

				//if (state == OPEN)
				//	drawLayout();
				//gl::drawStrokedRect(mainTitleButton->getButtonArea());
				gl::popMatrices();	

				if (state == OPEN)	
					CompositeDispatcher::draw();
				//	saveBtn->draw();							
			}

			virtual void drawContent()
			{
			}
			void setOpenLayoutIndex(int openLayoutIndex)
			{
				this->openLayoutIndex = openLayoutIndex;
			
				float time =  0.7f;
				EaseFn eFunc = EaseOutExpo();
				Vec2f animateTo;
				float height = 0;

				unActivateListeners();

				if(openLayoutIndex == index)
				{						
					openingLayout(eFunc, time);
					animateTo = Vec2f(0, openLayoutIndex * closeHeightMin);						
				}
				else if (openLayoutIndex == -1)
				{
					//activateListeners();
					closingLayoutMaxState(eFunc, time);						
					animateTo = Vec2f(0, index * closeHeightMax);								
				}	
				else
				{
					//activateListeners();
					closingLayoutMinState(eFunc, time);					

					if (openLayoutIndex > index)					
						animateTo =  Vec2f(0, index * closeHeightMin);					
					else
						animateTo =  Vec2f(0, (index -1)* (closeHeightMin) + openHeight);														
				}

				timeline().apply( &animatePosition, animateTo, time, eFunc)
					.updateFn(bind( &IPhotoboothItem::posAnimationUpdate, this))
						.finishFn( bind( &IPhotoboothItem::animationFinish2, this));
			}

			void openingLayout(EaseFn eFunc, float time)
			{
				state = OPEN;								
				onOpenResetParams();
				timeline().apply( &mainTitleY, openY, time, eFunc);
				timeline().apply( &subTitleY,  openY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight,  openHeight + 2, time, eFunc);	
			}

			void closingLayoutMaxState(EaseFn eFunc, float time)
			{
				state = CLOSE;		
				mainTitleButton->setButtonArea(Rectf(0.0f, 0.0f, itemWidth, closeHeightMax));
				timeline().apply( &mainTitleY, closeMaxY, time, eFunc);
				timeline().apply( &subTitleY,  closeMaxY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, closeHeightMax + 2, time, eFunc);			
			}

			void closingLayoutMinState(EaseFn eFunc, float time)
			{
				state = CLOSE_MIN;	
				mainTitleButton->setButtonArea(Rectf(0.0f, 0.0f, itemWidth, closeHeightMin));
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
				if(state != OPEN)
					activateListeners();
				//	saveBtn->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);				
			}

			virtual void onOpenResetParams()
			{

			}

		protected:
			enum {CLOSE, OPEN, CLOSE_MIN} state;
			Color color;
	
			Font mainTextFont, subTextFont;
			int itemWidth, index, openLayoutIndex;
			int closeHeightMin, closeHeightMax, openHeight;
			
			Texture mainTextTex, subTextTexClose, subTextTexOpen;


			PhotoboothSettingsRef settings;
			MainTitleButtonRef mainTitleButton;	
			SaveBtnRef saveBtn;

			Anim<Vec2f> animatePosition;
			Anim<float> mainTitleY, subTitleY;
			Anim<int> animHeight;

			float closeMaxY, closeMinY, openY;
			float offsetBetweenTitles;
		};

		typedef std::shared_ptr<IPhotoboothItem> IPhotoboothItemRef;
	}
}