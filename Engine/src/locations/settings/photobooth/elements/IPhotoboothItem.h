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
			typedef PhotoboothSettings::SettingsPartID SettingsPartID;
			typedef PhotoboothSettings::TextID TextID;
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, SettingsPartID id, Color color, int index)
				:CompositeDispatcher(),
				settings(phbSettings),
				state(CLOSE),
				index(index),
				itemWidth(1080 - 166),
				closeHeightMax(384),
				closeHeightMin(213),
				openHeight(1068),
				color(color),
				closeMinY(50),
				closeMaxY(130),
				openY(170),
				offsetBetweenTitles(60),
				animatePosition(Vec2f(0,0))
			{
				mainTitleY = closeMaxY;
				subTitleY  = closeMaxY + offsetBetweenTitles;
				animHeight = closeHeightMax;

				mainTitleButton = MainTitleButtonRef(new MainTitleButton(Rectf(0.0f, 0.0f, itemWidth, closeHeightMax), index));
				
				addChild(mainTitleButton);
				setPosition(Vec2f(0.0f, closeHeightMax * index));

				animatePosition = Vec2f(0.0f, closeHeightMax * index);
			
				mainTextTex		= textTools().getTextField(phbSettings->getMainTitle(id),
					&phbSettings->getFont("introLight44"), Color::white());

				subTextTex		= textTools().getTextField(phbSettings->getSubTitle(id),
					&phbSettings->getFont("helveticaLight24"), Color::hex(0xfff600));
	

				saveBtn = SaveBtnRef(new SaveBtn(Rectf(0.0f, 0.0f, 245.0f, 65.0f),
											     phbSettings->getText(TextID::SAVE_TEXT),
												 phbSettings->getFont("introLight30") )),	

				saveBtn->setPosition(Vec2f(0.5 * (itemWidth - 245.0f),  835.0f));
				addChild(saveBtn);
			}

			virtual void activateListeners()
			{
				mainTitleButton->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);	
				CompositeDispatcher::activateListeners();
			}

			virtual void unActivateListeners()
			{
				mainTitleButton->removeMouseUpListener();
				saveBtn->removeMouseUpListener();
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
					gl::draw(subTextTex, Vec2f(0.5  * (itemWidth - subTextTex.getWidth()), subTitleY));
				}
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

				if(openLayoutIndex == index)
				{					
					openingLayout(eFunc, time);
					animateTo = Vec2f(0, openLayoutIndex * closeHeightMin);						
				}
				else if (openLayoutIndex == -1)
				{
					closingLayoutMaxState(eFunc, time);						
					animateTo = Vec2f(0, index * closeHeightMax);								
				}	
				else
				{
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
				timeline().apply( &animHeight,  openHeight, time, eFunc);	
			}

			void closingLayoutMaxState(EaseFn eFunc, float time)
			{
				state = CLOSE;		
				mainTitleButton->setButtonArea(Rectf(0.0f, 0.0f, itemWidth, closeHeightMax));
				timeline().apply( &mainTitleY, closeMaxY, time, eFunc);
				timeline().apply( &subTitleY,  closeMaxY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, closeHeightMax, time, eFunc);			
			}

			void closingLayoutMinState(EaseFn eFunc, float time)
			{
				state = CLOSE_MIN;	
				mainTitleButton->setButtonArea(Rectf(0.0f, 0.0f, itemWidth, closeHeightMin));
				timeline().apply( &mainTitleY, closeMinY, time, eFunc);
				timeline().apply( &subTitleY,  closeMinY + offsetBetweenTitles, time, eFunc);
				timeline().apply( &animHeight, closeHeightMin, time, eFunc);
			}

			virtual void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

			void animationFinish2()
			{
				activateListeners();
				if(state == OPEN)			
					saveBtn->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);				
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
			
			Texture mainTextTex, subTextTex;
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