#include "states/choosing/PhotoContainer.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace shaders::imagefilters;

shaders::imagefilters::BaseShaderRef PhotoContainer::shader;

PhotoContainer::PhotoContainer(int id,
	const ci::gl::Texture& tex1, 
	const ci::gl::Texture& tex2, 
	const ci::Vec2f& vec)
	:SimpleSpriteButton(ci::Rectf(vec, vec + ci::Vec2f(270.0f, 350.0f)),
	PhotoChoosedEventRef(new PhotoChoosedEvent(id))),
	galka(tex1),
	ramka(tex2),
	isSelected(false)
{

}

void PhotoContainer::setSelected(bool value)
{
	isSelected = value;
}

bool PhotoContainer::selected()
{
	return isSelected;
}

void PhotoContainer::drawLayout()
{
	using namespace ci;

	gl::translate(animPosition);

	if (photo)
	{		
		gl::color(ci::ColorA(1.0f, 1.0f, 1.0f, alpha));
		gl::draw(photo);
		gl::color(ci::Color::white());

		if (isSelected)
		{
			gl::draw(ramka, Vec2f(0.5f * (photo.getWidth() - ramka.getWidth()), 0.5f * (photo.getHeight() - ramka.getHeight())));
			gl::draw(galka, Vec2f(29.0f, 270.0f));
		}
	}
}

void PhotoContainer::setPhoto(const ci::gl::Texture& tex)
{
	originphoto = tex;
}

void PhotoContainer::setShader(BaseShaderRef shader)
{
	this->shader = shader;

	DotScreenRef dotShader = std::dynamic_pointer_cast<DotScreen>(shader);
	if (dotShader)
	{
		dotShader->setScale(0.9f);
	}

	NoiseRef noiseShader = std::dynamic_pointer_cast<Noise>(shader);
	if (noiseShader)
	{
		noiseShader->setNoiseAmount(0.2f);
	}

	photo = Utils::drawGraphicsToFBO(originphoto.getSize(), [&]()
	{
		shader->setAlpha(1.0f);
		shader->render(originphoto);
	});		
}
