#pragma once
#include "BaseShader.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

namespace kubik
{
	namespace shaders
	{
		class ContrastShader : public BaseShader
		{		
			float contrast;
			float brightness;
			float multiple;		

		public:
			ContrastShader(float _contrast, float _brightness) : contrast(_contrast), brightness(_brightness)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());
				multiple = 1.0f;
				params = InterfaceGl::create(getWindow(), "Contrast Params", toPixels(Vec2i(300, 200)));
				params->addParam("contrast", &contrast).min(0.0f).max(10.0f).step(.1f);
				params->addParam("brightness", &brightness).min(0.0f).max(10.0f).step(.1f);
				params->addParam("multiple", &multiple).min(0.0f).max(10.0f).step(.1f);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
										 uniform sampler2D tex0;
                                         uniform float contrast;
                                         uniform float brightness;
                                         uniform float multiple;
                                         
                                         void main(){
                                             vec4 color = texture2D(tex0,gl_TexCoord[0].st);
                                             
                                             float p = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
                                             p = p * brightness;
                                             vec4 color2 = vec4(p,p,p,1.0);
                                             color *= color2;
                                             color *= vec4(multiple,multiple,multiple,1.0);
                                             color = mix( vec4(1.0,1.0,1.0,1.0),color,contrast);
                                             
                                             gl_FragColor =  vec4(color.r , color.g, color.b, 1.0);
                                         });

				return shdr.c_str();
			}

			void render(Surface surf)
			{
				Texture tex = Texture(surf);
				tex.bind(0);
				shader.bind();
				shader.uniform("basetex", 0);	
				shader.uniform("contrast", contrast);
				shader.uniform("brightness", brightness);
				shader.uniform("multiple", multiple);
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}
		};
		typedef shared_ptr<ContrastShader> ContrastShaderRef;
	}
}