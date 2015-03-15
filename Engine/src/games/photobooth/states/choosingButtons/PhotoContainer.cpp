#include "states/choosingButtons/PhotoContainer.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace ci;
using namespace std;
using namespace shaders::imagefilters;

shaders::imagefilters::BaseShaderRef PhotoContainer::shader;

PhotoContainer::PhotoContainer(int id, const ci::gl::Texture& tex1, const ci::gl::Texture& tex2, const ci::Vec2f& vec)
	:SimpleSpriteButton(Rectf(vec, vec + Vec2f(270, 350)),
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
	if(photo)
		gl::draw(photo);

	if (isSelected)
	{		
		gl::draw(ramka, Vec2f(-9.0f, -10.0f));	
		gl::draw(galka, Vec2f(29.0f, 270.0f));		
	}
}

void PhotoContainer::setPhoto(const ci::gl::Texture& tex)
{
	originphoto = tex;
}

void PhotoContainer::setShader(BaseShaderRef shader)
{
	this->shader = shader;

	gl::Fbo fbo = gl::Fbo(originphoto.getWidth(), originphoto.getHeight());

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		shader->render(originphoto);
	});

	photo = fbo.getTexture();
	Utils::clearFBO(fbo);
}