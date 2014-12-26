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
		class LutShader : public BaseShader
		{				    
			float opacity;

		public:
			LutShader(float _opacity) :opacity(_opacity)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());	

				params = InterfaceGl::create(getWindow(), "LutShader Params", toPixels(Vec2i(300, 200)));				
				params->addParam("opacity", &opacity).min(0.0f).max(1).step(.1f);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
					varying vec2 texcoord;
					uniform sampler2DRect tex;

					uniform sampler2DRect texLUT;

					void main (void)
					{
						vec4 textureColor = texture2DRect(tex, texcoord);
     
						float blueColor = textureColor.b * 63.0;
     
						vec2 quad1;
						quad1.y = floor(floor(blueColor) / 8.0);
						quad1.x = floor(blueColor) - (quad1.y * 8.0);
     
						vec2 quad2;
						quad2.y = floor(ceil(blueColor) / 8.0);
						quad2.x = ceil(blueColor) - (quad2.y * 8.0);
     
						vec2 texPos1;
						texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
						texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
     
						vec2 texPos2;
						texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
						texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
     
						vec4 newColor1 = texture2DRect(texLUT, texPos1*512.);
						vec4 newColor2 = texture2DRect(texLUT, texPos2*512.);
     
						vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
						gl_FragColor = vec4(newColor.rgb, textureColor.w);
     
					});

				return shdr.c_str();
			}

			void render(Surface surf)
			{
				Texture tex = Texture(surf);
				tex.bind(0);
				shader.bind();
				shader.uniform("tDiffuse", 0);	
				shader.uniform("opacity", opacity);			
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}
		};
		typedef shared_ptr<LutShader> LutShaderRef;
	}
}