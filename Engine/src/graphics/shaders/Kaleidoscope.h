#pragma once
#include "BaseShader.h"

namespace shaders
{
	namespace imagefilters
	{
		class Kaleidoscope : public BaseShader
		{
			float segments;

		public:
			Kaleidoscope(float _segments)
				:BaseShader("Kaleidoscope filter"), segments(_segments)
			{
				shader = ci::gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());
			}

			void createParams(ci::params::InterfaceGlRef params)
			{
				params->clear();
				params->addParam("segments", &segments).min(0).max(100).step(1);
			}

			const char *GET_FRAG() override
			{
				static const std::string shdr = STRINGIFY(
					uniform sampler2D tex;
				uniform float segments;
				uniform float alpha;

				void main()
				{
					vec2 uv = gl_TexCoord[0].st;
					vec2 normed = 2.0 * uv - 1.0;
					float r = length(normed);
					float theta = atan(normed.y / abs(normed.x));
					theta *= segments;

					vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;

					vec4 color = texture2D(tex, newUv);
					color.a = alpha;
					gl_FragColor = color;
				}
				);

				return shdr.c_str();
			}

			void render(const ci::gl::Texture& tex)
			{
				tex.bind(0);
				shader.bind();
				shader.uniform("tex", 0);
				shader.uniform("segments", segments);
				shader.uniform("alpha", alpha);
				ci::gl::drawSolidRect(tex.getBounds());
				shader.unbind();
				tex.unbind();
			}
		};
		typedef std::shared_ptr<Kaleidoscope> KaleidoscopeRef;
	}
}