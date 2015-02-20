#pragma once
#include "GameSettingsSprite.h"
#include "InstakubSettings.h"
#include "SearchBlock.h"
#include "HashTagBlock.h"
#include "PhotoCardStyle.h"

namespace kubik 
{
	namespace config
	{
		class InstakubConfig: public GameSettingsSprite
		{
		public:	
			InstakubConfig(InstakubSettingsRef instSettings)
				:GameSettingsSprite()			
			{					
				searchBlock  = SearchBlockRef(new SearchBlock(instSettings, Vec2i(0,0)));
				hashTagBlock = HashTagBlockRef(new HashTagBlock(instSettings, Vec2i(0, 435)));
				photoCardStyle   = PhotoCardStyleRef(new PhotoCardStyle(instSettings, Vec2i(0, 920)));
			
				addChild(searchBlock);
				addChild(hashTagBlock);
				addChild(photoCardStyle);								
			}			

			void mouseUpHandler(EventGUIRef& _event)
			{
				EventGUI *ev = _event.get();
				if(!ev)
					return;
		
				//if(typeid(*ev) == typeid(OpenPhotoBoothLayoutEvent))
				//{					
				//	OpenPhotoBoothLayoutEventRef event = static_pointer_cast<OpenPhotoBoothLayoutEvent>(_event);	
				//	setOpenItem(event->getlayoutIndex());
				//}
				//else if(typeid(*ev) == typeid(CheckerSocialEvent))
				//{	
				//	CheckerSocialEventRef event = static_pointer_cast<CheckerSocialEvent>(_event);	
				//	console()<<"CheckerSocialEvent:::: "<<event->getSocialID()<<" ....  "<<event->getValue()<<endl;
				//}
				//else if(typeid(*ev) == typeid(PhotoTemplateChooseEvent))
				//{
				//	PhotoTemplateChooseEventRef event = static_pointer_cast<PhotoTemplateChooseEvent>(_event);	
				//	console()<<"PhotoTemplateChooseEvent:::: "<<event->getCount()<<endl;
				//}
				//else if(typeid(*ev) == typeid(ChangePhotoOverDesignEvent))
				//{
				//	ChangePhotoOverDesignEventRef event = static_pointer_cast<ChangePhotoOverDesignEvent>(_event);	
				//	console()<<"PhotoTemplateChooseEvent:::: "<<event->getItem().getID()<<endl;
				//}
				//else if(typeid(*ev) == typeid(OpenSystemDirectoryEvent))
				//{
				//	OpenSystemDirectoryEventRef event = static_pointer_cast<OpenSystemDirectoryEvent>(_event);	
				//	console()<<"OpenSystemDirectoryEvent:::: "<<endl;
				//}
				//else if(typeid(*ev) == typeid(ChangePhotoCardStyleDesignEvent))
				//{
				//	ChangePhotoCardStyleDesignEventRef event = static_pointer_cast<ChangePhotoCardStyleDesignEvent>(_event);	
				//	console()<<"ChangePhotoCardStyleDesignEvent:::: "<<event->getItem().getID()<<endl;
				//}
				//else if(typeid(*ev) == typeid(ChangePhotoFilterPreviewActiveEvent))
				//{
				//	ChangePhotoFilterPreviewActiveEventRef event = static_pointer_cast<ChangePhotoFilterPreviewActiveEvent>(_event);	
				//	console()<<"ChangePhotoFilterPreviewActiveEvent:::: "<<endl;//event->getItem().getID()<<endl;
				//}				
			}
			void activateListeners()
			{
				for (auto layout : displayList)	
				{
					layout->connectEventHandler(&InstakubConfig::mouseUpHandler, this);
					layout->activateListeners();
				}	
			}

			void unActivateListeners()
			{
				for (auto layout : displayList)	
				{
					layout->disconnectEventHandler();
					layout->unActivateListeners();
				}
			}			

			void showAnimate(EaseFn eFunc, float time) 
			{	
				animatePosition = getGlobalPosition();
				unActivateListeners();
				ci::Vec2f finPos = ci::Vec2f(166.0f, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn(bind( &InstakubConfig::showAnimationFinish, this))
						.updateFn(bind( &InstakubConfig::animationPosUpdate, this));

				initKeyBoard();
			}

			void animationPosUpdate()
			{
				setPosition(animatePosition.value());					
			}

			void showAnimationFinish()
			{
				activateListeners();
			}

			void hideAnimate(ci::EaseFn eFunc, float time)
			{
				unActivateListeners();
				ci::Vec2f finPos = ci::Vec2f(1080.0f, 0.0f);
				timeline().apply( &animatePosition, finPos, time, eFunc)
						.finishFn(bind( &InstakubConfig::hideAnimationFinish, this))
						.updateFn(bind( &InstakubConfig::animationPosUpdate, this));

				//touchKeyboard().connectEventHandler(&NewActivityPopup::inputTouchHandler, this, VirtualKeyboard::HIDED);
				touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);	
				touchKeyboard().disconnectKeyboard();
				touchKeyboard().hide(ci::Vec2f(30, 1200), 0.3);
			}		

			void hideAnimationFinish()
			{		
				
			}


			void initKeyBoard()
			{	
				touchKeyboard().setOriginPoint(Vec2f(166.0f, 0.0f));
				touchKeyboard().setInputField(260.0f + 73.0f - 234 + 60, 260.0f + 435, 260.0f + 621.0f + 86.0f - 234 + 76, 260.0f + 102.0f+ 435);
				touchKeyboard().connectKeyboard();
				touchKeyboard().connectEventHandler(&InstakubConfig::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);				
			}

			void inputTouchHandler()
			{
				console()<<"inputTouchHandler"<<endl;
				touchKeyboard().show(ci::Vec2f(-130.0f, 800.0f), Vec2f(-130.0f, 760.0f), 0.7f);
			}

			virtual void draw()
			{
				Sprite::draw();
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
					touchKeyboard().draw();
				gl::popMatrices();				
			}

		private:					
			ci::Anim<ci::Vec2f> animatePosition;
			SearchBlockRef searchBlock;
			HashTagBlockRef hashTagBlock;
			PhotoCardStyleRef photoCardStyle;
		};

		typedef std::shared_ptr<InstakubConfig> InstakubConfigRef;
	}
}