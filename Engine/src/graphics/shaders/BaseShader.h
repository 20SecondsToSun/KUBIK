#pragma once
//#include "stdafx.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;
using namespace params;

#define STRINGIFY(s) #s

namespace kubik
{
	namespace shaders
	{	
	
		class BaseShader
		{

		protected:
			InterfaceGlRef	params;
			GlslProg shader;

		public:
			// The GOL vertex shader is a simple pass-through:
			const char *GET_PASSTHROUGH_VERTEX() const
			{	
				static const string shdr = STRINGIFY(
					void main() {
						gl_FrontColor  = gl_Color;
						gl_TexCoord[0] = gl_MultiTexCoord0;
						gl_Position    = ftransform();
				}
				);

				return shdr.c_str();
			}

			virtual const char * GET_FRAG() = 0;
			virtual void render(Surface surf) = 0;

			void drawParams()
			{
				params->draw();
			}

			void hideParams()
			{
				params->hide();
			}

			void showParams()
			{
				params->show();
			}
		};
	}
}