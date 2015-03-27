#include "KeyBoardButtonSprite.h"
using namespace kubik;


KeyBoardButtonSprite::KeyBoardButtonSprite(const ci::gl::Texture& image, ci::Font _font, const std::string& _label)
	:SimpleSpriteButton(image.getWidth(), image.getHeight(), Vec2f::zero()), 
	image(image),
	overColor(Color::white()),
	code(_label),
	font(_font),
	isTextField(true)
{
	createTextField();
	alpha = 1;
}

KeyBoardButtonSprite::KeyBoardButtonSprite(ci::gl::Texture image, std::string _label)
	:SimpleSpriteButton(image.getWidth(), image.getHeight(), Vec2f::zero()), 
	image(image),
	overColor(Color::white()),
	code(_label),
	isTextField(false)
{
	alpha = 1;
}

void KeyBoardButtonSprite::drawLayout()
{
	gl::color(Color::white());
	gl::color(Utils::colorAlpha(overColor.value(), alpha));
	gl::draw(image, Vec2f(0.0f, 3.0f));
	gl::color(Color::white());

	if (textTexture)
		gl::draw(textTexture,
		Vec2f(0.5f*(buttonArea.getWidth() - textTexture.getWidth()),
		0.5f*(buttonArea.getHeight() - textTexture.getHeight())));
	gl::color(Color::white());
}

void KeyBoardButtonSprite::changeTexture(const Texture& image)
{
	this->image = image;
}

std::string KeyBoardButtonSprite::getBtnId() const
{
	return code;
}

void  KeyBoardButtonSprite::setBtnId(const std::string& value)
{
	if (isTextField)
	{
		code = value;
		createTextField();
	}
}

void  KeyBoardButtonSprite::createTextField()
{
	TextLayout simple;
	simple.setFont(font);
	simple.setColor(Color::black());
	simple.addLine(Utils::cp1251_to_utf8(code.c_str()));
	textTexture = gl::Texture(simple.render(true, false));
}

void KeyBoardButtonSprite::down()
{
	timeline().apply(&overColor, Color::hex(0x00b6c4), Color::white(), 0.5f, EaseInBack());
	//.finishFn([&]()
	//{
	//});
}

void KeyBoardButtonSprite::setAlpha(float alpha)
{
	this->alpha = alpha;
}