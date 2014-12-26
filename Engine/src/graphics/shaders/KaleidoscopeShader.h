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
		class KaleidoscopeShader : public BaseShader
		{		
			float segments;
		public:
			KaleidoscopeShader(float _segments) : segments(_segments)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());

				params = InterfaceGl::create(getWindow(), "Kaleid Params", toPixels(Vec2i(300, 200)));
				params->addParam("segments", &segments).min(0).max(100).step(1);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
					uniform sampler2D tex;
					uniform float segments;

					void main()
					{
						vec2 uv = gl_TexCoord[0].st;
						vec2 normed = 2.0 * uv - 1.0;
						float r = length(normed);
						float theta = atan(normed.y / abs(normed.x));
						theta *= segments;

						vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;

						gl_FragColor = texture2D(tex, newUv);
					}
				);

				return shdr.c_str();
			}

			void render(Surface surf)
			{
				Texture tex = Texture(surf);
				tex.bind(0);
				shader.bind();			
				shader.uniform("tex", 0);	
				shader.uniform("segments", segments);		
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}
		};
		typedef shared_ptr<KaleidoscopeShader> KaleidoscopeShaderRef;
	}
}