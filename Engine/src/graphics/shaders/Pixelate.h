#pragma once

#include "BaseShader.h"

namespace shaders
{	
	namespace imagefilters
	{
		class Pixelate : public BaseShader
		{		    
			ci::Vec2f resolution;			

		public:
			Pixelate(const ci::Vec2f& resolution) 
				:BaseShader("Pixelate filter"),
				resolution(resolution)
			{
				shader = ci::gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());			
			}

			void createParams(ci::params::InterfaceGlRef params)
			{
				params->clear();
				params->addParam("resolution.x", &this->resolution.x).min(0.0f).max(1000.0f).step(1.0f);
				params->addParam("resolution.y", &this->resolution.y).min(0.0f).max(1000.0f).step(1.0f);				
			}

			const char *GET_FRAG() override
			{
				static const std::string shdr = STRINGIFY(
					uniform sampler2D tex;
					uniform float xPixels;
					uniform float yPixels;
					uniform float alpha;

				void main()
				{
					vec2 texCoords = vec2(floor(gl_TexCoord[0].s * xPixels) / xPixels, floor(gl_TexCoord[0].t * yPixels) / yPixels);
					
					vec4 color = texture2D(tex, texCoords);
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
				shader.uniform("alpha", alpha);
				shader.uniform("xPixels", resolution.x);
				shader.uniform("yPixels", resolution.y);
				ci::gl::drawSolidRect(tex.getBounds());
				shader.unbind();
				tex.unbind();
			}
		};
		typedef std::shared_ptr<Pixelate> PixelateRef;
	}
}