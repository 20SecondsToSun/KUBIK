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
		class BloomShader : public BaseShader
		{		
			float bloom;

		public:
			BloomShader(float _bloom) : bloom(_bloom)
			{
				shader = gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());

				params = InterfaceGl::create(getWindow(), "Bloom Params", toPixels(Vec2i(300, 200)));
				params->addParam("bloom", &bloom).min(-10.1f).max(100.0f).step(.1f);
				params->hide();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(
					    // varying vec2 texcoord;  
						uniform sampler2D tex0;
						uniform float bloom;  

						void main()  
						{  
							vec2 vUv = gl_TexCoord[0].st;
							vec4 base = texture2D(tex0, vUv);

							vec4 sum;						 
							int i;
							int j; 
							
							for( i = -3 ;i <= 3; i++)    
							{    
								for (j = -3; j <= 3; j++)    
								{    
									sum += texture2D(tex0, vUv  + vec2(j, i) * 2.0) * 0.25;  // 2.0 = "radius step"
								}    
							}    

							 sum = sum * sum * 0.012; // 0.012 = arbitrary value   
							 vec4 dry = texture2D(tex0, vUv);
							 gl_FragColor = bloom * (1.0 - ((1.0 - sum) * (1.0 - dry))) +  ((1.0 - bloom) * dry); // screen mode blending  
						});

				return shdr.c_str();
			}

			void render(Surface surf)
			{
				Texture tex = Texture(surf);
				tex.bind(0);
				shader.bind();
				gl::enableAlphaBlending();
				shader.uniform("tex0", 0);	
				shader.uniform("bloom", bloom);
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}	
		};
		typedef shared_ptr<BloomShader> BloomShaderRef;
	}
}