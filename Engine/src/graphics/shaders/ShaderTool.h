#pragma once

#include "shaders/BaseShader.h"
#include "shaders/Pixelate.h"
#include "shaders/HueSaturation.h"
#include "shaders/MaskShader.h"
#include "shaders/Sepia.h"
#include "shaders/Noise.h"
#include "shaders/ColorHalftone.h"
#include "shaders/DotScreen.h"
#include "shaders/Kaleidoscope.h"
#include "shaders/Normal.h"
#include "shaders/Brcosa.h"

namespace shaders
{	
	namespace imagefilters
	{
		class ShaderTool
		{
		public:
			enum FilterType
			{
				_start,
				NORMAL,
				BRCOSA,
				SEPIA,
				DOT_SCREEN,
				HUE_SATURATION,
				NOISE,		
				MASK,	
				_NULL
			};

			static ShaderTool& getInstance();

			ShaderTool();
			MaskShaderRef getMaskShader();
			BaseShaderRef get(FilterType id);
			BaseShaderRef get(FilterType id, ci::params::InterfaceGlRef params);
			BaseShaderRef getNext(ci::params::InterfaceGlRef params);
			BaseShaderRef getPrev(ci::params::InterfaceGlRef params);

			void setdefaultValues();

		private:
			FilterType currentID;
			MaskShaderRef maskShader;
			std::map<FilterType, BaseShaderRef> shadermap;			
		};

		inline ShaderTool&	shadertool() { return ShaderTool::getInstance(); };
	}
}