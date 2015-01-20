#pragma once
#include "IDispatcher.h"
#include "PhotoboothSettings.h"
#include "photobooth/elements/MainTitleButton.h"
#include "SaveBtn.h"
#include "cinder/Timeline.h"

namespace kubik
{
	namespace config
	{
		class IPhotoboothItem: public IDispatcher
		{
		public:
			typedef PhotoboothSettings::SettingsPartID SettingsPartID;
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, SettingsPartID id, Color color, int index)
				:settings(phbSettings),
				state(CLOSE),
				index(index),
				itemWidth(1080-166),
				closeHeightMax(384),
				closeHeightMin(213),
				openHeight(1068),
				color(color),
				closeMinY(50),
				closeMaxY(130),
				openY(170),
				mainTitleY(closeMaxY),
				subTitleY(closeMaxY + 60),
				animHeight(closeHeightMax)
			{	
				mainTitleButton = MainTitleButtonRef(new MainTitleButton(index));
				mainTextTex		= textTools().getTextField(phbSettings->getMainTitle(id), &phbSettings->getFont("introLight44"), Color::white());
				subTextTex		= textTools().getTextField(phbSettings->getSubTitle(id), &phbSettings->getFont("helveticaLight24"), Color::hex(0xfff600));

				saveBtn = SaveBtnRef(new SaveBtn(phbSettings->getSaveText(), phbSettings->getFont("introLight30")));						
			}

			void activateListeners()
			{
				mainTitleButton->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);			
			}

			void addSaveBtnListener()
			{
				saveBtn->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);
			}

			void unActivateListeners()
			{
				mainTitleButton->removeMouseUpListener();
			}

			void removeSaveBtnListener()
			{
				saveBtn->removeMouseUpListener();
			}

			void setPosition(Vec2i position)
			{
				initPosition = position;
				position += Vec2f(0.0f, index * closeHeightMax);
				float __x = position.x + 0.5 * (itemWidth - 245);
				float __y = 835;

				animatePosition = position;
				saveBtn->setButtonArea(Rectf(__x, __y + position.y, __x + 245, __y + position.y + 65));
				mainTitleButton->setButtonArea(Rectf(position.x, position.y, position.x + itemWidth, position.y + closeHeightMax));
				IDispatcher::setPosition(position);
			}

			virtual void draw()
			{				
				gl::pushMatrices();
				gl::translate(animatePosition);
				gl::color(color);
				gl::drawSolidRect(Rectf(0, 0, itemWidth, animHeight));
				gl::color(Color::white());
				gl::draw(mainTextTex, Vec2f(0.5 * (itemWidth - mainTextTex.getWidth()), mainTitleY));
				if (state == CLOSE || state == CLOSE_MIN)
				{								
					gl::draw(subTextTex, Vec2f(0.5  * (itemWidth - subTextTex.getWidth()), subTitleY));
				}					
				gl::popMatrices();	
				if (state == OPEN)
				{
					drawContent();
					saveBtn->draw();
				}
			}

			virtual void drawContent()
			{

			}

			void removeMainButtonListener()
			{
			}

			void setOpenLayoutIndex(int openLayoutIndex)
			{
				this->openLayoutIndex = openLayoutIndex;

				float time =  0.5f;
				EaseFn eFunc = EaseOutCubic();
				Vec2f pos;
				float height = 0;

				if(openLayoutIndex == index)
				{
					state = OPEN;
					unActivateListeners();					
					onOpenResetParams();

					pos = initPosition + Vec2f(0, openLayoutIndex * closeHeightMin);					
				
					timeline().apply( &mainTitleY, openY, time, eFunc);
					timeline().apply( &subTitleY,  openY + 60, time, eFunc);
					timeline().apply( &animHeight,  openHeight, time, eFunc);	
				}
				else
				{						
					removeSaveBtnListener();					

					if (openLayoutIndex == -1)
					{
						state = CLOSE;
						height = closeHeightMax;
						pos = initPosition + Vec2f(0, index * height);

						timeline().apply( &mainTitleY, closeMaxY, time, eFunc);
						timeline().apply( &subTitleY,  closeMaxY + 60, time, eFunc);
						timeline().apply( &animHeight, closeHeightMax, time, eFunc);					
					}	
					else
					{
						state = CLOSE_MIN;
						height = closeHeightMin;
						if (openLayoutIndex > index)					
							pos = initPosition + Vec2f(0, index * height);					
						else
							pos = initPosition + Vec2f(0, (index -1)* (height) + openHeight);	

						timeline().apply( &mainTitleY, closeMinY, time, eFunc);
						timeline().apply( &subTitleY,  closeMinY + 60, time, eFunc);
						timeline().apply( &animHeight, closeHeightMin, time, eFunc);		
					}								
				}

				timeline().apply( &animatePosition, pos, time, eFunc)
						.finishFn( bind( &IPhotoboothItem::animationFinish2, this, pos, height));
			}

			void animationFinish2(Vec2f pos, float height)
			{
				if(state != OPEN)
				{
					mainTitleButton->setButtonArea(Rectf(pos.x, pos.y, pos.x + itemWidth, pos.y + height));
					activateListeners();
				}
				else
					addSaveBtnListener();

				IDispatcher::setPosition(pos);
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
			Vec2f initPosition;

			Texture mainTextTex, subTextTex;
			PhotoboothSettingsRef settings;
			MainTitleButtonRef mainTitleButton;	
			SaveBtnRef saveBtn;

			Anim<Vec2f> animatePosition;
			Anim<float> mainTitleY, subTitleY;
			Anim<int> animHeight;

			float closeMaxY, closeMinY, openY;
		};

		typedef std::shared_ptr<IPhotoboothItem> IPhotoboothItemRef;
	}
}