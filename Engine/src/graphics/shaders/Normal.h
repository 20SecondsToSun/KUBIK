#pragma once
#include "BaseShader.h"

namespace shaders
{
	namespace imagefilters
	{
		class Normal : public BaseShader
		{		
		public:
			Normal():BaseShader("Normal filter")
			{
				using namespace ci;
				shader = ci::gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());
			}

			void createParams(ci::params::InterfaceGlRef params)
			{
				params->clear();
			}

			const char *GET_FRAG()
			{
				static const std::string shdr = STRINGIFY(
					uniform sampler2D texture;		

				void main()
				{
					vec2 vUv = gl_TexCoord[0].st;
					vec4 color = texture2D(texture, vUv);			
					gl_FragColor = color;
				});

				return shdr.c_str();
			}

			void render(const ci::gl::Texture& tex)
			{
				ci::gl::draw(tex);			
			}
		};
		typedef std::shared_ptr<Normal> NormalRef;
	}
}