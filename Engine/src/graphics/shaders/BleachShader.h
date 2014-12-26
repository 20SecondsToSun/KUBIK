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
		class BleachShader : public BaseShader
		{				    
			float opacity;

		public:
			BleachShader(float _opacity) :opacity(_opacity)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());	

				params = InterfaceGl::create(getWindow(), "Bleach Params", toPixels(Vec2i(300, 200)));				
				params->addParam("opacity", &opacity).min(0.0f).max(1).step(.1f);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
					uniform float opacity;
					uniform sampler2D tDiffuse;
					void main() {
					vec2 vUv = gl_TexCoord[0].st;
					vec4 base = texture2D( tDiffuse, vUv );

					vec3 lumCoeff = vec3( 0.25, 0.65, 0.1 );
					float lum = dot( lumCoeff, base.rgb );
					vec3 blend = vec3( lum );

					float L = min( 1.0, max( 0.0, 10.0 * ( lum - 0.45 ) ) );

					vec3 result1 = 2.0 * base.rgb * blend;
					vec3 result2 = 1.0 - 2.0 * ( 1.0 - blend ) * ( 1.0 - base.rgb );

					vec3 newColor = mix( result1, result2, L );

					float A2 = opacity * base.a;
					vec3 mixRGB = A2 * newColor.rgb;
					mixRGB += ( ( 1.0 - A2 ) * base.rgb );

					gl_FragColor = vec4( mixRGB, base.a );
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
		typedef shared_ptr<BleachShader> BleachShaderRef;
	}
}