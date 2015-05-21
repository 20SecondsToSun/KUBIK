#include "states/template/PhotoTemplate.h"

using namespace kubik;
using namespace kubik::games::photobooth;
using namespace ci;
using namespace std;
using namespace shaders::imagefilters;

BaseShaderRef TemplateButton::shader;

TemplateButton::TemplateButton(const Rectf& rect, templateID id, const vector<Texture>& templates, const vector<Texture>& stickers)
	:SimpleSpriteButton(rect, PhotoTemplateChooseEventRef(new PhotoTemplateChooseEvent(id))),
	templates(templates),
	stickers(stickers),
	isSelected(false),
	id(id)
{

}

void TemplateButton::setPhotoTemplates(const vector<map<FormatID, Texture>>& photoTemplates, BaseShaderRef shader)
{
	this->photoTemplates = photoTemplates;
	this->shader = shader;
}

void TemplateButton::setSelected(bool value)
{
	isSelected = value;
}

templateID TemplateButton::getID()
{
	return id;
}

void TemplateButton::drawLayout()
{
	gl::translate(animPosition);

	gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
	shader->setAlpha(alpha);
	drawTemplateBg();
	drawPhotos();
	drawStickers();

	if (isSelected)
	{
		gl::color(ColorA(0.0f, 0.0f, 0.0f, 0.75f));
		gl::drawSolidRect(buttonArea);
		gl::color(Color::white());
		gl::draw(btnOver, (getSize() - btnOver.getSize()) * 0.5f);
	}
}

void TemplateButton::setSelectDesign(const Texture& btn)
{
	btnOver = btn;
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton1
//
////////////////////////////////////////////////////////////////////////////

TemplateButton1::TemplateButton1(const Rectf& rect, const vector<Texture>& templates, const vector<Texture>& stickers)
	:TemplateButton(rect, FIRST, templates, stickers)
{
	_scale = 218.0f / templates[0].getWidth();
	shiftY = 4.0f;
	_scale1 = ((float)templates[0].getWidth() / stickers[2].getWidth());
}

void TemplateButton1::drawTemplateBg()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::draw(templates[0]);
	gl::popMatrices();
}

void TemplateButton1::drawPhotos()
{
	gl::pushMatrices();
	shader->render(photoTemplates[0][FormatID::FORMAT1_SMALL]);
	gl::translate(0, 195.0f);
	shader->render(photoTemplates[1][FormatID::FORMAT1_SMALL]);
	gl::translate(0, 195.0f);
	shader->render(photoTemplates[2][FormatID::FORMAT1_SMALL]);
	gl::popMatrices();
}

void TemplateButton1::drawStickers()
{
	gl::pushMatrices();
	gl::scale(_scale1, _scale1);
	gl::scale(_scale, _scale);
	gl::draw(stickers[2]);
	gl::translate(0, stickers[2].getHeight() + shiftY);
	gl::draw(stickers[2]);
	gl::translate(0, stickers[2].getHeight() + shiftY);
	gl::draw(stickers[2]);
	gl::popMatrices();
}

ci::gl::Texture TemplateButton1::getPrintTemplate()
{
	return Utils::drawGraphicsToFBO(templates[0].getSize(), [&]()
	{
		float shiftY = 5.0f;
		float _height = photoTemplates[0][FormatID::FORMAT1_PRINT].getHeight();
		gl::draw(templates[0]);
		gl::pushMatrices();		
		shader->render(photoTemplates[0][FormatID::FORMAT1_PRINT]);
		gl::translate(0, _height + 5);
		shader->render(photoTemplates[1][FormatID::FORMAT1_PRINT]);
		gl::translate(0, _height + 5);
		shader->render(photoTemplates[2][FormatID::FORMAT1_PRINT]);
		gl::popMatrices();		
		gl::scale(_scale1, _scale1);
		gl::draw(stickers[2]);
		gl::translate(0.0f, stickers[2].getHeight() + shiftY);
		gl::draw(stickers[2]);
		shiftY = 5.0f;
		gl::translate(0.0f, stickers[2].getHeight() + shiftY);
		gl::draw(stickers[2]);
	});
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton2
//
////////////////////////////////////////////////////////////////////////////

TemplateButton2::TemplateButton2(const Rectf& rect, const vector<Texture>& templates, const vector<Texture>& stickers)
	:TemplateButton(rect, SECOND, templates, stickers)
{
	_scale = 303.0f / templates[1].getWidth();
	_scale1 = (float)templates[1].getWidth() / stickers[0].getWidth();

	float startY = 517.0f, endY = 603.0f;
	subBtns.clear();
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(16.0f,  startY, 83.0f, endY),  SUB_FIRST)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(118.0f, startY, 185.0f, endY), SUB_SECOND)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(220.0f, startY, 287.0f, endY), SUB_THIRD)));

	for (unsigned int i = 0; i < subBtns.size(); i++)
	{
		subBtns[i]->setParent(this);
		subBtns[i]->setChildPosition(_localPosition);
		subBtns[i]->setAlpha(alpha);
	}
}

void TemplateButton2::init()
{
	selectedTemplate = nullptr;
	activeIndex = 0;

	console() << "photoTemplates  " << photoTemplates.size() << endl;
	console() << "subBtns  " << subBtns.size() << endl;

	for (unsigned int i = 0; i < subBtns.size(); i++)
	{
		console() << "subBtns init :::::::: " << i << endl;
		subBtns[i]->setPhoto(photoTemplates[i][FormatID::FORMAT2_SMALL], shader);// DEBUG ASSERT TODO
	}
		
}

void TemplateButton2::setSelected(bool value)
{
	isSelected = value;

	if (isSelected)
	{
		if (!selectedTemplate)
			selectedTemplate = subBtns[0];

		selectedTemplate->setSelected(true);
	}
	else if (selectedTemplate)
		selectedTemplate->setSelected(false);
}

void TemplateButton2::activateListeners()
{
	for (auto btn : subBtns)
		btn->connectEventHandler(&TemplateButton2::photoTemplateChoose, this);

	TemplateButton::activateListeners();
}

void TemplateButton2::unActivateListeners()
{
	for (auto btn : subBtns)
		btn->disconnectEventHandler();

	TemplateButton::unActivateListeners();
}

void TemplateButton2::photoTemplateChoose(EventGUIRef& _event)
{
	auto eventref = static_pointer_cast<SubPhotoTemplateChooseEvent>(_event);
	activeIndex = eventref->getTemplateID();

	if (selectedTemplate)
		selectedTemplate->setSelected(false);

	selectedTemplate = subBtns[activeIndex];
	selectedTemplate->setSelected(true);

	if (!isSelected)
		mouseUpSignal(event);
}

void TemplateButton2::drawTemplateBg()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::draw(templates[1]);
	gl::popMatrices();
}

void TemplateButton2::drawPhotos()
{
	shader->render(photoTemplates[activeIndex][FormatID::FORMAT2_BIG]);
	
	for (unsigned int i = 0; i < subBtns.size(); i++)
	{
		gl::pushMatrices();
		gl::translate(subBtns[i]->getLocalPosition());
		subBtns[i]->drawLayout();
		gl::popMatrices();
	}
}

void TemplateButton2::drawStickers()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::scale(_scale1, _scale1);
	gl::draw(stickers[0]);
	gl::popMatrices();

	gl::draw(lineTexture, Vec2f(49.0f, getHeight() + 14.0f));
	gl::draw(lineTexture, Vec2f(153.0f, getHeight() + 14.0f));
	gl::draw(lineTexture, Vec2f(246.0f, getHeight() + 14.0f));
}

void TemplateButton2::setSelectRamkaTexture(const Texture& texture)
{
	for (unsigned int i = 0; i < subBtns.size(); i++)
		subBtns[i]->setSelectRamkaTexture(texture);
}

void TemplateButton2::setLineTexture(const Texture& texture)
{
	lineTexture = texture;
}

ci::gl::Texture TemplateButton2::getPrintTemplate()
{
	return Utils::drawGraphicsToFBO(templates[1].getSize(), [&]()
	{
		gl::draw(templates[1]);
		gl::pushMatrices();
		shader->render(photoTemplates[activeIndex][FormatID::FORMAT2_PRINT]);
		//gl::scale(_scale, _scale);
		gl::scale(_scale1, _scale1);
		gl::draw(stickers[0]);
		gl::popMatrices();
	});
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton3
//
////////////////////////////////////////////////////////////////////////////

TemplateButton3::TemplateButton3(const Rectf& rect, const vector<Texture>& templates, const vector<Texture>& stickers)
	:TemplateButton(rect, THIRD, templates, stickers)
{
	_scale = 303.0f / templates[3].getWidth();
	_scale1 = ((665.0f - 77.0f) / stickers[1].getWidth());
	shiftX = 4.0f;
}

void TemplateButton3::drawTemplateBg()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::draw(templates[3]);
	gl::popMatrices();
}

void TemplateButton3::drawPhotos()
{
	gl::pushMatrices();
	shader->render(photoTemplates[0][FormatID::FORMAT4_BIG]);
	gl::translate(101.5f, 0);
	shader->render(photoTemplates[1][FormatID::FORMAT4_BIG]);
	gl::translate(101.5f, 0);
	shader->render(photoTemplates[2][FormatID::FORMAT4_BIG]);
	gl::popMatrices();
}

void TemplateButton3::drawStickers()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::scale(_scale1, _scale1);
	gl::draw(stickers[1]);
	gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
	gl::draw(stickers[1]);
	gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
	gl::draw(stickers[1]);
	gl::popMatrices();
}

ci::gl::Texture TemplateButton3::getPrintTemplate()
{
	return Utils::drawGraphicsToFBO(templates[3].getSize(), [&]()
	{
		gl::draw(templates[3]);
		gl::pushMatrices();
		float _width = photoTemplates[0][FormatID::FORMAT4_PRINT].getWidth();
		shader->render(photoTemplates[0][FormatID::FORMAT4_PRINT]);
		gl::translate(_width + 4, 0);
		shader->render(photoTemplates[1][FormatID::FORMAT4_PRINT]);
		gl::translate(_width + 4, 0);
		shader->render(photoTemplates[2][FormatID::FORMAT4_PRINT]);
		gl::popMatrices();

		gl::pushMatrices();		
		gl::scale(_scale1, _scale1);
		gl::draw(stickers[1]);
		gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
		gl::draw(stickers[1]);
		gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
		gl::draw(stickers[1]);
		gl::popMatrices();
	});
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton4
//
////////////////////////////////////////////////////////////////////////////

TemplateButton4::TemplateButton4(const Rectf& rect, const vector<Texture>& templates, const vector<Texture>& stickers)
	:TemplateButton(rect, FOURTH, templates, stickers)
{
	_scale = 202.0f / templates[2].getWidth();
	_scale1 = ((float)templates[2].getWidth() / stickers[3].getWidth());
	_scale2 = 101.0f / (812.0f - 610.0f);
	shiftY = 1.0f;
}

void TemplateButton4::drawTemplateBg()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::draw(templates[2]);
	gl::popMatrices();
}

void TemplateButton4::drawPhotos()
{
	shader->render(photoTemplates[0][FormatID::FORMAT3_BIG]);
	gl::pushMatrices();
	gl::translate(Vec2f(0.0f, 203.5f));
	shader->render(photoTemplates[1][FormatID::FORMAT3_SMALL]);
	gl::popMatrices();
}

void TemplateButton4::drawStickers()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::scale(_scale1, _scale1);
	gl::draw(stickers[3]);
	gl::translate(0.0f, stickers[3].getHeight() + shiftY);
	gl::scale(_scale2, _scale2);
	gl::draw(stickers[3]);
	gl::popMatrices();
}

ci::gl::Texture TemplateButton4::getPrintTemplate()
{
	return Utils::drawGraphicsToFBO(templates[2].getSize(), [&]()
	{
		gl::pushMatrices();
		gl::draw(templates[2]);
		gl::popMatrices();
		shader->render(photoTemplates[0][FormatID::FORMAT3_PRINT_1]);
		gl::pushMatrices();
		gl::translate(Vec2f(0.0f, photoTemplates[0][FormatID::FORMAT3_PRINT_1].getHeight() + 4));
		shader->render(photoTemplates[1][FormatID::FORMAT3_PRINT_2]);
		gl::popMatrices();

		gl::pushMatrices();
		gl::scale(_scale1, _scale1);
		gl::translate(0.0f, -1.0f);
		gl::draw(stickers[3]);
		gl::translate(0.0f, stickers[3].getHeight() + shiftY + 1);
		gl::scale(_scale2, _scale2);
		gl::draw(stickers[3]);
		gl::popMatrices();
	});
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton5
//
////////////////////////////////////////////////////////////////////////////

TemplateButton5::TemplateButton5(const Rectf& rect, const vector<Texture>& templates, const vector<Texture>& stickers)
	:TemplateButton(rect, FIFTH, templates, stickers)
{
	_scale = 303.0f / templates[4].getWidth();
	_scale1 = 917.0f / stickers[0].getWidth();
	_scale2 = 101.0f / stickers[3].getWidth();
}

void TemplateButton5::drawTemplateBg()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::draw(templates[4]);
	gl::popMatrices();
}

void TemplateButton5::drawPhotos()
{
	shader->render(photoTemplates[0][FormatID::FORMAT2_MIDDLE]);

	gl::pushMatrices();
	gl::translate(Vec2f(158.0f, 0.0f));
	shader->render(photoTemplates[0][FormatID::FORMAT3_SMALL]);	
	gl::translate(Vec2f(0.0f, 102.0f));
	shader->render(photoTemplates[1][FormatID::FORMAT3_SMALL]);
	gl::popMatrices();
}

void TemplateButton5::drawStickers()
{
	gl::pushMatrices();
	gl::scale(_scale, _scale);
	gl::scale(_scale1, _scale1);
	gl::draw(stickers[0]);
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate(158.0f, 0.0f);
	gl::scale(_scale2, _scale2);
	gl::draw(stickers[3]);
	gl::translate(0.0f, stickers[3].getHeight());
	gl::draw(stickers[3]);
	gl::popMatrices();
}

ci::gl::Texture TemplateButton5::getPrintTemplate()
{
	return Utils::drawGraphicsToFBO(templates[4].getSize(), [&]()
	{
		gl::pushMatrices();
		gl::draw(templates[4]);
		gl::popMatrices();
		shader->render(photoTemplates[0][FormatID::FORMAT5_PRINT_1]);
		gl::pushMatrices();
		gl::translate(Vec2f(photoTemplates[0][FormatID::FORMAT5_PRINT_1].getWidth() + 5, 0.0f));
		shader->render(photoTemplates[0][FormatID::FORMAT5_PRINT_2]);
		gl::translate(Vec2f(0.0f, 593.0f));
		shader->render(photoTemplates[1][FormatID::FORMAT5_PRINT_2]);
		gl::popMatrices();

		gl::pushMatrices();
		gl::scale(_scale1, _scale1);
		gl::draw(stickers[0]);
		gl::popMatrices();

		float _scale2 = 588.0f / stickers[3].getWidth();
		gl::pushMatrices();
		gl::translate(922.0f, 0.0f);
		gl::scale(_scale2, _scale2);
		gl::draw(stickers[3]);
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(922.0f, 4.0f);
		gl::scale(_scale2, _scale2);		
		gl::translate(0.0f, stickers[3].getHeight());
		gl::draw(stickers[3]);
		gl::popMatrices();
	});
}

////////////////////////////////////////////////////////////////////////////
//
//					SubButton
//
////////////////////////////////////////////////////////////////////////////

SubButton::SubButton(const Rectf& rect, subID id)
	:SimpleSpriteButton(rect, SubPhotoTemplateChooseEventRef(new SubPhotoTemplateChooseEvent(id))),
	selected(false)
{

}

void SubButton::drawLayout()
{
	if (photo)
		gl::draw(photo);

	if (selected)
		gl::draw(ramka);
}

void SubButton::setSelectRamkaTexture(const Texture& texture)
{
	ramka = texture;
}

void SubButton::setSelected(bool value)
{
	selected = value;
}

void SubButton::setPhoto(const Texture& tex, BaseShaderRef shader)
{
	gl::Fbo fbo = gl::Fbo(tex.getWidth(), tex.getHeight());

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		shader->render(tex);
	});

	photo = fbo.getTexture();
	Utils::clearFBO(fbo);
}