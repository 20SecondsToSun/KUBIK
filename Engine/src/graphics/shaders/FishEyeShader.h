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
		class FishEyeShader : public BaseShader
		{		
			float fishEyeLensradius;
			float fishEyeSigncurvature;	

		public:
			FishEyeShader(float _fishEyeLensradius, float _fishEyeSigncurvature) 
				: fishEyeLensradius(_fishEyeLensradius), fishEyeSigncurvature(_fishEyeSigncurvature)
			{
				shader = gl::GlslProg(GET_VERT(), GET_FRAG());

				params = InterfaceGl::create(getWindow(), "FishEye Params", toPixels(Vec2i(300, 200)));
				params->addParam("fishEyeLensradius", &fishEyeLensradius).min(0.0f).max(10.0f).step(0.1f);
				params->addParam("fishEyeSigncurvature", &fishEyeSigncurvature).min(0.0f).max(10.0f).step(0.1f);				
				params->hide();
			}
			const char *GET_VERT()
			{
				static const string shdr = STRINGIFY(
				varying vec2 texcoord0;
				varying vec2 texcoord1;
				varying vec2 texdim0;
				varying vec2 texdim1;
				varying vec2 texorient0;
				varying vec2 texorient1;

				void main()
				{
					// perform standard transform on vertex
					gl_Position = ftransform();					

					// transform texcoords
					texcoord0 = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
					texcoord1 = vec2(gl_TextureMatrix[1] * gl_MultiTexCoord1);
					// extract the x and y scalars from the texture matrix to determine dimensions
					texdim0 = vec2 (abs(gl_TextureMatrix[0][0][0]),abs(gl_TextureMatrix[0][1][1]));
					texdim1 = vec2 (abs(gl_TextureMatrix[1][0][0]),abs(gl_TextureMatrix[1][1][1]));
					// extract the sign for orientation
					texorient0 = vec2 (gl_TextureMatrix[0][0][0]/texdim0.x,gl_TextureMatrix[0][1][1]/texdim0.y);
					texorient1 = vec2 (gl_TextureMatrix[1][0][0]/texdim1.x,gl_TextureMatrix[1][1][1]/texdim1.y);
				});

				return shdr.c_str();
			}

			const char *GET_FRAG() override
			{
				static const string shdr = STRINGIFY(				
				varying vec2 texcoord0;
				varying vec2 texdim0;
				uniform float lensradius;
				uniform float signcurvature;
				uniform sampler2D tex0;
				void main()
				{
					float epsilon = 0.000011;
					float curvature = abs(signcurvature);
					float extent = lensradius;
					float optics = extent / log2(curvature * extent + 1.0) / 1.4427;
					vec2 vUv = gl_TexCoord[0].st;
					vec2 normalizeTD = vUv / texdim0;
					vec2 PP = normalizeTD - vec2(0.5,0.5);
					float P0 = PP[0];
					float P1 = PP[1];
					float radius = sqrt(P0 * P0 + P1 * P1);

					//float cosangle = P0 / radius;
					//float sinangle = P1 / radius;

					//float rad1, rad2, newradius;
					//rad1 = (exp2((radius / optics) * 1.4427) - 1.0) / curvature;
					//rad2 = optics * log2(1.0 + curvature * radius) / 1.4427;
					//newradius = signcurvature > 0.0 ? rad1 : rad2;

					//vec2 FE = vec2(newradius * cosangle + 0.5,newradius * sinangle + 0.5);
					//FE = radius <= extent ? FE : normalizeTD;
					////FE = curvature < epsilon ? normalizeTD : FE;

					//gl_FragColor = texture2DRect(tex0.rgb, FE*texdim0);
				});

				return shdr.c_str();
			}

			void render(Surface surf)
			{
				Texture tex = Texture(surf);
				tex.bind(0);
				shader.bind();
				shader.uniform("tex0", 0);	
				shader.uniform("fishEyeLensradius", fishEyeLensradius);
				shader.uniform("fishEyeSigncurvature", fishEyeSigncurvature);
				gl::draw(surf);
				shader.unbind();
				tex.unbind();
			}
		};
		typedef shared_ptr<FishEyeShader> FishEyeShaderRef;
	}
}