#pragma once

#include "IPhotoboothLocation.h"
#include "CameraAdapter.h"
#include "shaders/PixelateShader.h"
#include "shaders/ContrastShader.h"
#include "shaders/BleachShader.h"
#include "shaders/KaleidoscopeShader.h"
#include "shaders/BloomShader.h"
#include "shaders/BrcosaShader.h"
#include "shaders/FishEyeShader.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;
using namespace kubik::shaders;
using namespace params;

namespace kubik
{
	class PhotoTimer:public IPhotoboothLocation
	{
		Texture fon;
		Font font;
		Font font100;
		int seconds;
		Timer cdTimer;	

		PixelateShdrRef pixelateShdr;
		ContrastShaderRef contrastShdr;
		BleachShaderRef bleachShdr;
		KaleidoscopeShaderRef kalShdr;
		BloomShaderRef bloomShdr;
		BrcosaShaderRef brcosaShdr;
		FishEyeShaderRef fishEyeShdr;
		
		BaseShader *shader;
		
		InterfaceGlRef params;
	public:

		void clear()
		{
			cdTimer.stop();
		}

		~PhotoTimer()
		{
			clear();
		};

		PhotoTimer(PhotoboothSettingsRef settings)
		{		
			reset(settings);
			setParams();
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings =  _settings;
			fon		 =  settings->getTexture("fon3");
			font	 =  settings->getFont("helvetica40");
			font100	 =  settings->getFont("helvetica100");			
		}

		void setParams()
		{
			pixelateShdr = PixelateShdrRef(new PixelateShader(Vec2f(100.0f, 100.0f)));
			contrastShdr = ContrastShaderRef(new ContrastShader(1, 0.4f));
			bleachShdr   = BleachShaderRef(new BleachShader( 2.0f));
			kalShdr      = KaleidoscopeShaderRef(new KaleidoscopeShader(2));
			bloomShdr    = BloomShaderRef(new BloomShader(1.0f));
			brcosaShdr   = BrcosaShaderRef(new BrcosaShader(0.2f, 0.7f, 0.9f, 1.0f));
			//fishEyeShdr  = FishEyeShaderRef(new FishEyeShader(2, 4));
			shader		 = bloomShdr.get();			

			params = InterfaceGl::create(getWindow(), "Choose Shader", toPixels(Vec2i(300, 400)));			
			params->addButton("Bloom",  bind(&PhotoTimer::cooseShader, this, 1));
			params->addButton("Brcosa", bind(&PhotoTimer::cooseShader, this, 2));
			params->addButton("Kaleid", bind(&PhotoTimer::cooseShader, this, 3));
			params->addButton("Pixel",  bind(&PhotoTimer::cooseShader, this, 4));
			params->addButton("Constrast", bind(&PhotoTimer::cooseShader, this, 5));
			params->addButton("Bleach",    bind(&PhotoTimer::cooseShader, this, 6));
			//params->addButton("Fish Eye",  bind(&PhotoTimer::cooseShader, this, 7));
			params->setPosition(Vec2i(690, 0));
			params->hide();
		}

		void cooseShader(int num)
		{
			shader->hideParams();
			switch (num)
			{
				case 1:
					shader		 = bloomShdr.get();
					break;

				case 2:					
					shader		 = brcosaShdr.get();
				break;

				case 3:
					shader		 = kalShdr.get();
				break;

				case 4:
					shader		 = pixelateShdr.get();
				break;

				case 5:
					shader		 = contrastShdr.get();
				break;

				case 6:
					shader		 = bleachShdr.get();
				break;

				case 7:
					shader		 = fishEyeShdr.get();
				break;

				default:
					break;
			}
			shader->showParams();
		}
		void start()
		{
			seconds = settings->getData().seconds;
			//cdTimer.start();
			console()<<"start PhotoTimer"<<endl;	
			shader->showParams();
			params->show();
		}		

		void update() override
		{
			seconds = (settings->getData().seconds - (int)cdTimer.getSeconds());

			if(seconds < 0)
			{
				clear();
				nextLocationSignal();
			}

			cameraCanon().update();
		}

		void draw() override
		{
			float scale = 0.7f;
			gl::draw(fon, getWindowBounds());

			gl::pushMatrices();	
			gl::translate(0, 200);
			gl::scale(scale, scale);
			shader->render(cameraCanon().getLiveSurface());
			gl::popMatrices();

			gl::pushMatrices();				
			gl::translate(1024 * scale, 200);
			gl::scale(scale, scale);
			cameraCanon().draw();
			gl::popMatrices();	

			shader->drawParams();
			params->draw();

			textTools().textFieldDraw("“¿…Ã≈–", &font, Color::white(), Vec2f(100.0f, 100.0f));
			textTools().textFieldDraw(to_string(seconds), &font100, Color::white(), Vec2f(200.0f, 200.0f));
		}

		void mouseUpHandler(Vec2i vec)
		{
			clear();
			nextLocationSignal();
		}
	};

	typedef shared_ptr<PhotoTimer> PhotoTimerRef;
}