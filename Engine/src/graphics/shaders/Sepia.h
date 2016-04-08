#pragma once

#include "BaseShader.h"

namespace shaders
{	
	namespace imagefilters
	{
		class Sepia : public BaseShader
		{	    
			float amount;	
		
		public:
			Sepia(float amount)
				:BaseShader("Sepia filter"), amount(amount)
			{
				using namespace ci;
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());	
			}

			void createParams(ci::params::InterfaceGlRef params)
			{
				params->clear();
				params->addParam("amount", &this->amount).min(0.0f).max(1.0f).step(.1f);
			}

			const char *GET_FRAG() override
			{
				static const std::string shdr = STRINGIFY(
					  uniform sampler2D texture;
					  uniform float amount;
					  uniform float alpha;

					  void main()
					  {
						vec2 vUv = gl_TexCoord[0].st;							
						vec4 color = texture2D(texture, vUv);		
				
						float r = color.r;
						float g = color.g;
						float b = color.b;
           
					   color.r = min(1.0, (r * (1.0 - (0.607 * amount))) + (g * (0.769 * amount)) + (b * (0.189 * amount)));
					   color.g = min(1.0, (r * 0.349 * amount) + (g * (1.0 - (0.314 * amount))) + (b * 0.168 * amount));
					   color.b = min(1.0, (r * 0.272 * amount) + (g * 0.534 * amount) + (b * (1.0 - (0.869 * amount))));   
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
		typedef std::shared_ptr<Sepia> SepiaRef;
	}
}