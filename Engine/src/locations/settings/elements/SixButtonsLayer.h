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
		template <class Type> class SixButtonsLayer : public Sprite
		{
		public:
			SixButtonsLayer(const SixButtonsInitObject& object)
			{
				using namespace ci;

				Vec2f pos = Vec2f::zero();
				Vec2f shiftVector(53, 130);
				int i = 0;				
				userDesignID = object.getUserDesignID();
				//activeID = object.getActiveID();

				for (auto it : object.getDesignData())
				{
					SixButtonOneData oneButtondata(it, object.getBgTextColor(), object.getOverTexture());
					oneButtondata.calculateOffset(object.getStartVec(), shiftVector, i++);

					ImageQuadroButtonRef imageQuadroButton = std::shared_ptr<SixItemButton<Type>>(new SixItemButton<Type>(oneButtondata));
					btns[it.getID()] = imageQuadroButton;
					addChild(imageQuadroButton);		
				}

				loadButton = settingsFactory().createDecorLoadButton(object.getPath(), btns[userDesignID]->getLocalPosition() + Vec2f(0.0f, btns[userDesignID]->getHeight()), object.getOverTexture());
				selectActiveDesign(object.getActiveID());
			}

			void selectActiveDesign(int id)
			{
				if (activeID == id) return;

				activeID = id;

				if (activeBtn)
				{
					if (activeBtn->getItem().getID() == userDesignID)
					{
						removeChild(loadButton);
						loadButton->disconnectEventHandler();
					}

					activeBtn->setSelection(false);
				}

				btns[id]->setSelection(true);
				activeBtn = btns[id];

				if (id == userDesignID)// && !hasChild(loadButton))
				{
					addChild(loadButton);
					//loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);
				}
			}

			virtual void buttonClicked(EventGUIRef& event)
			{
				EventGUI *ev = event.get();

				if (typeid(*ev) == typeid(Type))
				{
					//ChangePhotoCardStyleDesignEventRef statEvent = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(event);	
					int id = static_pointer_cast<Type>(event)->getItem().getID();
					btns[id]->setSelection(true);

					if (activeID != id && id == userDesignID)
						loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);

					selectActiveDesign(id);
				}
			}

			void openSystemDirectory(EventGUIRef& event)
			{

			}

			virtual void activateListeners()
			{
				for (auto it : btns)
					it.second->connectEventHandler(&SixButtonsLayer::buttonClicked, this);

				if (activeBtn && activeBtn->getItem().getID() == userDesignID)
					loadButton->connectEventHandler(&SixButtonsLayer::openSystemDirectory, this);
			}

			virtual void unActivateListeners()
			{
				for (auto it : btns)
					it.second->disconnectEventHandler();

				loadButton->disconnectEventHandler();
			}

		private:
			std::map<int, ImageQuadroButtonRef> btns;
			ImageQuadroButtonRef activeBtn;
			LoadButtonRef loadButton;
			int userDesignID, activeID;
		};
	}
}