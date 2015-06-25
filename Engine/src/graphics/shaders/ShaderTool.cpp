#include "shaders/ShaderTool.h"

using namespace shaders::imagefilters;
using namespace ci;

ShaderTool& ShaderTool::getInstance()
{
	static ShaderTool sht;
	return sht;
};

ShaderTool::ShaderTool()
{
	shadermap[NORMAL]		  = NormalRef(new Normal());
	shadermap[BRCOSA]		  = BrcosaRef(new Brcosa(0.0f, 1.0f, 1.0f, 1.0f));
	shadermap[SEPIA]		  = SepiaRef(new Sepia(0.6f));
	shadermap[DOT_SCREEN]	  = DotScreenRef(new DotScreen(Vec2i(100, 100), 1.7f, 0.6f));
	shadermap[HUE_SATURATION] = HueSaturationRef(new HueSaturation(1.0f, 0.4f));
	shadermap[NOISE]		  = NoiseRef(new Noise(0.5f));

	maskShader = MaskShaderRef(new MaskShader());
}

MaskShaderRef ShaderTool::getMaskShader()
{
	return maskShader;
}

BaseShaderRef ShaderTool::get(FilterType id)
{
	currentID = id;
	return shadermap[id];
}

BaseShaderRef ShaderTool::get(FilterType id, ci::params::InterfaceGlRef params)
{
	auto shader = get(id);
	shader->createParams(params);
	return shader;
}

BaseShaderRef ShaderTool::getNext(ci::params::InterfaceGlRef params)
{
	int cur = currentID;
	if (++cur == _NULL)	cur = 0;

	currentID = static_cast<FilterType>(cur);
	return get(currentID, params);
}

BaseShaderRef ShaderTool::getPrev(ci::params::InterfaceGlRef params)
{
	int cur = currentID;
	if (--cur == -1) cur = _NULL - 1;

	currentID = static_cast<FilterType>(cur);
	return get(currentID, params);
}