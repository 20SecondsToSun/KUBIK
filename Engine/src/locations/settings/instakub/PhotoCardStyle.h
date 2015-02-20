#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "InstakubSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoCardStyle> PhotoCardStyleRef;

		class PhotoCardStyle: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;

			PhotoCardStyle(InstakubSettingsRef settings, Vec2i position):Sprite()
			{
				setPosition(position);	

				titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_MAIN));
				titleTextPos = ci::Vec2f(0.5 * (914 - titleTextTex.getWidth()), 127.0f - 77 + 52);			

				subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_SUB));
				subTitleTextPos = ci::Vec2f(0.5 * (914 - subTitleTextTex.getWidth()), 179.0f - 13);	

				DesignData designdata = settings->getPhotoCardStyles();				

				ci::Vec2f pos = ci::Vec2f::zero();
				int i = 0;
				float shiftX = 53, shiftY = 130;
				float startX = 106, startY= 354;

				for (auto it : designdata)
				{				 
					Texture icon = settings->getTexture(it.getName());
					it.setIcon(icon);
					it.setFont(settings->getFonts());

					pos.x = startX + (icon.getWidth() + shiftX) * (i % 3);
					pos.y = startY + (icon.getWidth() + shiftY) * (i / 3);

					ImageQuadroButtonRef imageQuadroButton = settingsFactory().createCardStyleButton(it, pos);

					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);	
					i++;					
				}

				int id = settings->getActivePhotoCardStyleDesignID();
				userDesignID = settings->getUserPhotoCardStyleDesignID();
				
				console()<<"-----------btns size:::  "<<btns.size()<<endl;
				loadButton = settingsFactory().createDecorLoadButton(btns[userDesignID]->getLocalPosition() + Vec2f(0, btns[userDesignID]->getHeight()));			
				selectActiveDesign(id);	
			}

			virtual void drawLayout()
			{
				gl::color(Color::hex(0x578d24));
				gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(914, 1000)));
				gl::color(Color::white());

				gl::draw(titleTextTex, titleTextPos);
				gl::draw(subTitleTextTex, subTitleTextPos);	

				Sprite::drawLayout();
			}

			void selectActiveDesign(int id)
			{
				activeID = id;

				if(activeBtn)
				{
					if(activeBtn->getItem().getID() == userDesignID)
					{
						removeChild(loadButton);
						loadButton->disconnectEventHandler();
					}

					activeBtn->setSelection(false);
				}

				btns[id]->setSelection(true);
				activeBtn = btns[id];

				if(id == userDesignID)
				{
					addChild(loadButton);
					loadButton->connectEventHandler(&PhotoCardStyle::openSystemDirectory, this);
				}
			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&PhotoCardStyle::buttonClicked, this);

				if(activeBtn->getItem().getID() == userDesignID)
					loadButton->connectEventHandler(&PhotoCardStyle::openSystemDirectory, this);
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();	

				loadButton->disconnectEventHandler();
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if(typeid(*ev) == typeid(ChangePhotoCardStyleDesignEvent))
				{		
					ChangePhotoCardStyleDesignEventRef statEvent = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(event);	
					int id = statEvent->getItem().getID();
					btns[id]->setSelection(true);
					selectActiveDesign(id);	
				}
			}

			int getDesignID()
			{
				return activeID;
			}

			void openSystemDirectory(EventGUIRef& event)
			{
				console()<<"openSystemDirectory "<<endl;
			}

			private:
			std::map<int, ImageQuadroButtonRef> btns;	
			int userDesignID;
			int activeID;

			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;

			InstakubSettingsRef settings;	
			gl::Texture titleTextTex, subTitleTextTex;	
			ci::Vec2f titleTextPos, subTitleTextPos;
		};
	}
}