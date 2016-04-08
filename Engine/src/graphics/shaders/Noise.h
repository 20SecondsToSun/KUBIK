#pragma once

#include "BaseShader.h"

namespace shaders
{	
	namespace imagefilters
	{
		class Noise : public BaseShader
		{	    
			float amount;	
		
		public:
			Noise(float amount)
				:BaseShader("Noise filter"), amount(amount)
			{
				using namespace ci;
				shader = ci::gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());		
			}

			void createParams(ci::params::InterfaceGlRef params)
			{
				params->clear();
				params->addParam("amount", &this->amount).min(0.0f).max(1.0f).step(.1f);
			}

			const char *GET_FRAG()
			{
				static const std::string shdr = STRINGIFY(
					  uniform sampler2D texture;
					  uniform float amount;
					  uniform float alpha;
					  float rand(vec2 co) 
					  {
						 return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
					  }

					  void main()
					  {
						vec2 vUv = gl_TexCoord[0].st;							
						vec4 color = texture2D(texture, vUv);		       
      
						float diff = (rand(vUv) - 0.5) * amount;
						color.r += diff;
						color.g += diff;
						color.b += diff;
						color.a = alpha;
						
						gl_FragColor = color;
					});

				return shdr.c_str();
			}

			void render(const ci::gl::Texture& tex)
			{				
				tex.bind(0);
				shader.bind();
				shader.uniform("texture", 0);				
				shader.uniform("amount", amount);
				shader.uniform("alpha", alpha);
				ci::gl::drawSolidRect(tex.getBounds());
				shader.unbind();
				tex.unbind();
			}
		};
		typedef std::shared_ptr<Noise> NoiseRef;
	}
}