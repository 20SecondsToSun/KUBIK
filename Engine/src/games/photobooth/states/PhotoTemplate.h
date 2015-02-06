#pragma once

#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "cinder/gl/Texture.h"
#include "model/PhotoStorage.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	static const int PHOTO_COUNT_FOR_CHOOSING_TEMPLATE = 3;

	class PhotoTemplate: public IPhotoboothLocation
	{
		Texture fon, stickerTex, printTempaleTex;
		PhotoStorageRef photoStorage;
		Font font;		
		bool isPrint;

		vector<PhotoButtonRef> choosingTemplatesBtns;
		PhotoButton *lastSelectedPhotoButton;
		MenuButtonRef nextButton;

	public:
		PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage):photoStorage(photoStorage)
		{		
			reset(settings);		
		}

		~PhotoTemplate()
		{
			//disconnect buttons
		}		

		void start() override
		{
			console()<<"start PhotoTemplate"<<endl;

			photoStorage->createSharingPhotos(stickerTex);	

			if(isPrint)
				photoStorage->createPrintTemplatePhoto(printTempaleTex, stickerTex);

			if(!skipScreen())							
				createChoosingTemplatesButtons();				
			else
				nextLocationSignal();				
		}

		bool skipScreen()
		{
			return !(settings->getData().photoNum == PHOTO_COUNT_FOR_CHOOSING_TEMPLATE && isPrint);
		}

		void createChoosingTemplatesButtons()
		{
			vector<Surface> printTemplates = photoStorage->getPrintTemplates();
			float shift = 0.0f;
			int i = 0;
			choosingTemplatesBtns.clear();
			for (auto temp : printTemplates)	
			{
				PhotoButtonRef button = PhotoButtonRef(new PhotoButton(i++, temp, Vec2f(shift, 80.0f)));	
				connect_once(button->mouseUpSignal, bind(&PhotoTemplate::mouseUpListener, this, placeholders::_1));
				choosingTemplatesBtns.push_back(button);
				shift += temp.getWidth() + 20.0f;
			}

			if(!choosingTemplatesBtns.empty())
			{
				lastSelectedPhotoButton = choosingTemplatesBtns[0].get();	
				lastSelectedPhotoButton->setSelection(true);
			}
		}

		void mouseUpListener(PhotoButton& button)
		{
			lastSelectedPhotoButton->setSelection(false);
			lastSelectedPhotoButton = &button;
			lastSelectedPhotoButton->setSelection(true);
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			//typedef PhotoboothSettings::SocialID SocialID;

			//settings		= _settings;
			//fon				= settings->getTexture("fon1");
			//font			= settings->getFont("helvetica40");
			//stickerTex		= settings->getActiveStickerTex();	
			//printTempaleTex	= settings->getActivePrintBgTex();	
			////isPrint			= settings->getData().sharing.getSocialState(SocialID::PRINT);

			//nextButton = MenuButtonRef(new MenuButton(Rectf(800.0f, 700.0f, 900.0f, 800.0f), "ÄÀËÅÅ", font, (GameId)1));	
			////connect_once(nextButton->mouseUpSignal, bind(&PhotoTemplate::mouseUpNextListener, this, placeholders::_1));	
		}

		void mouseUpNextListener(MenuButton& button)
		{
			nextLocationSignal();
		}

		void update() override
		{

		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("ÂÛÁÅÐÈÒÅ ØÀÁËÎÍ", &font, Color::white(), Vec2f(50.0f, 50.0f));

			for (auto temp : choosingTemplatesBtns)			
				temp->draw();

			nextButton->draw();
		}

		void mouseUpHandler(Vec2i vec) override
		{
			for (auto temp : choosingTemplatesBtns)		
				temp->mouseUpHandler(vec);

			nextButton->mouseUpHandler(vec);
		}
	};

	typedef	shared_ptr<PhotoTemplate> PhotoTemplateRef;
}