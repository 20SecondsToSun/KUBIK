#pragma once
#include "IDispatcher.h"
#include "PhotoboothSettings.h"
#include "photobooth/elements/MainTitleButton.h"

namespace kubik
{
	namespace config
	{
		class IPhotoboothItem: public IDispatcher
		{
		public:	
			IPhotoboothItem(PhotoboothSettingsRef phbSettings, int index)
				:settings(phbSettings),
				state(CLOSE),
				index(index),
				itemWidth(1080-166)
			{				
				mainTitleButton = MainTitleButtonRef(new MainTitleButton(Rectf(0,0,0,0), index));
						
			}

			void activateListeners()
			{
				mainTitleButton->addMouseUpListener(&IPhotoboothItem::mouseUpFunction, this);			
			}

			void unActivateListeners()
			{
				mainTitleButton->removeMouseUpListener();
			}

			void setParams( Color color, string mainText,
							string subText, Font mainTextFont, Font subTextFont,
							int closeHeigtMin, int closeHeigtMax, int oneItemOpenHeight)
			{				
				this->color = color;
				this->mainText = mainText;
				this->subText = subText;
				this->mainTextFont = mainTextFont;
				this->subTextFont = subTextFont;
				this->closeHeigtMin = closeHeigtMin;
				this->closeHeigtMax = closeHeigtMax;
				this->openHeight = oneItemOpenHeight;

				mainTextTex = textTools().getTextField(mainText, &mainTextFont, Color::white());
				subTextTex = textTools().getTextField(subText, &subTextFont, Color::hex(0xfff600));

				this->position = Vec2f(0.0f, index * closeHeigtMax);
				mainTitleButton->setButtonArea(Rectf(this->position.x, this->position.y, this->position.x + itemWidth, this->position.y + closeHeigtMax));
			}

			void setPosition(Vec2i position)
			{
				initPosition = position;
				position += Vec2f(0.0f, index * closeHeigtMax);
				mainTitleButton->setButtonArea(Rectf(position.x, position.y, position.x + itemWidth, position.y + closeHeigtMax));
				IDispatcher::setPosition(position);
			}

			virtual void draw()
			{
				gl::pushMatrices();
				gl::translate(position);
				if (state == CLOSE)
				{				
					gl::color(color);
					gl::drawSolidRect(Rectf(0, 0, itemWidth, closeHeigtMax));
					gl::color(Color::white());
					gl::draw(mainTextTex, Vec2f(0.5 * (itemWidth - mainTextTex.getWidth()), 110.0f));					
					gl::draw(subTextTex, Vec2f(0.5  * (itemWidth - subTextTex.getWidth()), 171.0f));					
				}
				else if (state == CLOSE_MIN)
				{
					gl::color(color);
					gl::drawSolidRect(Rectf(0, 0, itemWidth, closeHeigtMin));
					gl::color(Color::white());
					gl::draw(mainTextTex, Vec2f(0.5 * (itemWidth - mainTextTex.getWidth()), 31.0f));					
					gl::draw(subTextTex, Vec2f(0.5  * (itemWidth - subTextTex.getWidth()), 92.0f));	
				}
				else if (state == OPEN)
				{
					gl::color(color);
					gl::drawSolidRect(Rectf(0, 0, itemWidth, openHeight));
					gl::color(Color::white());
					gl::draw(mainTextTex, Vec2f(0.5 * (itemWidth - mainTextTex.getWidth()), 110.0f));	
				}
				gl::popMatrices();
			}

			void removeMainButtonListener()
			{
			}

			void setOpenLayoutIndex(int openLayoutIndex)
			{
				this->openLayoutIndex = openLayoutIndex;

				if(openLayoutIndex == index)
				{
					state = OPEN;
					unActivateListeners();
					Vec2f pos = initPosition + Vec2f(0, openLayoutIndex * closeHeigtMin);					
					IDispatcher::setPosition(pos);
				}
				else
				{
					state = CLOSE_MIN;
					activateListeners();					
					Vec2f pos;
					if (openLayoutIndex > index)					
						pos = initPosition + Vec2f(0, index * closeHeigtMin);					
					else
						pos = initPosition + Vec2f(0, (index -1)* (closeHeigtMin) + openHeight);	
					
					mainTitleButton->setButtonArea(Rectf(pos.x, pos.y, pos.x + itemWidth, pos.y + closeHeigtMin));
					IDispatcher::setPosition(pos);
				}
			}

		protected:
			enum {CLOSE, OPEN, CLOSE_MIN} state;
			Color color;
			string mainText, subText;
			Font mainTextFont, subTextFont;
			int itemWidth, index, openLayoutIndex;
			int closeHeigtMin, closeHeigtMax, openHeight;
			Vec2f initPosition;

			Texture mainTextTex, subTextTex;
			PhotoboothSettingsRef settings;
			MainTitleButtonRef mainTitleButton;			
		};

		typedef std::shared_ptr<IPhotoboothItem> IPhotoboothItemRef;
	}
}