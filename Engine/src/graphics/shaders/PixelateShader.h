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
		class PixelateShader : public BaseShader
		{		    
			Vec2f resolution;			

		public:
			PixelateShader(const Vec2f& _resolution) : resolution(_resolution)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());	

				params = InterfaceGl::create(getWindow(), "Pixelate Params", toPixels(Vec2i(300, 200)));
				params->addParam("resolution.x", &resolution.x).min(0).max(1000).step(1);
				params->addParam("resolution.y", &resolution.y).min(0).max(1000).step(1);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
					uniform sampler2D tex;
					uniform float xPixels;
					uniform float yPixels;

				void main()
				{
					vec2 texCoords = vec2(floor(gl_TexCoord[0].s * xPixels) / xPixels, floor(gl_TexCoord[0].t * yPixels) / yPixels);
					gl_FragColor = texture2D(tex, texCoords);
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
				shader.uniform("xPixels", resolution.x);
				shader.uniform("yPixels", resolution.y);			
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}
		};
		typedef shared_ptr<PixelateShader> PixelateShdrRef;
	}
}