#pragma once

namespace shaders
{	
	namespace imagefilters
	{
		class MaskShader 
		{	   
			
		public:
			MaskShader()
			{
				shader = ci::gl::GlslProg(GET_PASSTHROUGH_VERTEX(), GET_FRAG());			
			}


			const char *GET_FRAG()
			{
				static const std::string shdr = STRINGIFY(
					uniform sampler2D tex;
					uniform sampler2D mask;		
					uniform int inverse;

				void main()
				{
					 vec4 color = texture2D(tex, gl_TexCoord[0].st);                                             
                 	 vec4 maskcolor = texture2D(mask, gl_TexCoord[0].st); 

					 if (maskcolor.rgb == vec3(0, 0, 0) && inverse == 0)
						 color.a = 0;
					 else if (maskcolor.rgb == vec3(1, 1, 1)  && inverse == 1)
						 color.a = 0;
					// if (maskcolor.a == 0 && inverse == 0)
					//	 color.a = 0;
					// else if (maskcolor.a == 1 && inverse == 1)
					//	 color.a = 0;

					 

						gl_FragColor = color;
				}
				);

				return shdr.c_str();
			}

			// The GOL vertex shader is a simple pass-through:
			const char *GET_PASSTHROUGH_VERTEX() const
			{	
				static const std::string shdr = STRINGIFY(
					void main() {
						gl_FrontColor  = gl_Color;
						gl_TexCoord[0] = gl_MultiTexCoord0;
						gl_Position    = ftransform();
				}
				);

				return shdr.c_str();
			}

			void render(ci::gl::Texture tex, ci::gl::Texture mask, ci::Vec2f position = ci::Vec2f::zero(), int inverse = 0)
			{
				using namespace ci;
				gl::pushMatrices();
				gl::translate(position);
				tex.bind(0);
				mask.bind(1);
				shader.bind();
				shader.uniform("inverse", inverse);
				shader.uniform("tex", 0);
				shader.uniform("mask", 1);	
				ci::gl::drawSolidRect(tex.getBounds());
				shader.unbind();
				tex.unbind();
				gl::popMatrices();
			}
		protected:			
			ci::gl::GlslProg shader;
		};

		typedef std::shared_ptr<MaskShader> MaskShaderRef;
	}
}