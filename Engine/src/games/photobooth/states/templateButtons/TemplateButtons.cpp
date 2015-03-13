#include "states/PhotoTemplate.h"

using namespace kubik;
using namespace ci;
using namespace std;
using namespace shaders::imagefilters;

BaseShaderRef TemplateButton::shader;

TemplateButton::TemplateButton(const Rectf& rect, templateID id, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:SimpleSpriteButton(rect, PhotoTemplateChooseEventRef(new PhotoTemplateChooseEvent(id))),
	templates(templates),
	stickers(stickers),
	isSelected(false),
	id(id)
{

}

void TemplateButton::setPhotoTemplates(const vector<std::map<FormatID, ci::gl::Texture>>& photoTemplates, BaseShaderRef shader)
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
	if(isSelected)	
	{
		gl::color(ColorA(0.0f, 0.0f, 0.0f, 0.75f));//::hex(0xe4cf97));		
		//drawtool().drawStrokedRect(Rectf(0, 0, getWidth(), getHeight()), 4);
		gl::drawSolidRect(Rectf(0, 0, getWidth(), getHeight()));
		gl::color(Color::white());
		gl::draw(btnOver, Vec2f((getWidth() - btnOver.getWidth()) * 0.5f, (getHeight() - btnOver.getHeight()) * 0.5f));
		//gl::draw(btnOverText, Vec2f((getWidth() - btnOverText.getWidth()) * 0.5f, (getHeight() - btnOverText.getHeight()) * 0.5f));
	}
}

void TemplateButton::setSelectDesign(const gl::Texture& btn)
{
	btnOver = btn;
	//btnOverText = btntext;
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton1
//
////////////////////////////////////////////////////////////////////////////

TemplateButton1::TemplateButton1(const ci::Rectf& rect, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:TemplateButton(rect, FIRST, templates, stickers)
{
	_scale = 218.0f / templates[0].getWidth();
	shiftY = 4.0f;
	_scale1 = ((float)templates[0].getWidth() / stickers[2].getWidth());
}

void TemplateButton1::drawLayout()
{	
	gl::pushMatrices();	
	gl::scale(_scale, _scale);	
	gl::draw(templates[0]);	
	gl::popMatrices();

	auto tex = photoTemplates[0][FormatID::FORMAT1_SMALL];
	gl::pushMatrices();	
	//gl::draw(tex);	
	shader->render(tex);
	gl::translate(0, 195);
	shader->render(tex);
	//gl::draw(tex);
	gl::translate(0, 195);
	shader->render(tex);
	//gl::draw(tex);	
	gl::popMatrices();

	gl::pushMatrices();	
	gl::scale(_scale1, _scale1);
	gl::scale(_scale, _scale);	
	gl::draw(stickers[2]);	
	gl::translate(0, stickers[2].getHeight() + shiftY);
	gl::draw(stickers[2]);
	gl::translate(0, stickers[2].getHeight() + shiftY);
	gl::draw(stickers[2]);	
	gl::popMatrices();

	TemplateButton::drawLayout();
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton2
//
////////////////////////////////////////////////////////////////////////////

TemplateButton2::TemplateButton2(const ci::Rectf& rect, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:TemplateButton(rect, SECOND, templates, stickers)
{
	_scale  = 303.0f / templates[1].getWidth();
	_scale1 = (float)templates[1].getWidth() / stickers[0].getWidth();

	float startY = 517.0f, endY = 603.0f;
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(16.0f,  startY, 83.0f,  endY), SUB_FIRST)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(118.0f, startY, 185.0f, endY), SUB_SECOND)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(220.0f, startY, 287.0f, endY), SUB_THIRD)));
	
	addChild(subBtns[0]);
	addChild(subBtns[1]);
	addChild(subBtns[2]);
}

void TemplateButton2::init()
{
	selectedTemplate = nullptr;
	activeIndex = 0;

	for (int i = 0; i < subBtns.size(); i++)	
		subBtns[i]->setPhoto(photoTemplates[i][FormatID::FORMAT2_SMALL], shader);
}

void TemplateButton2::setSelected(bool value)
{
	isSelected = value;

	if (isSelected)
	{
		if(!selectedTemplate)	
			selectedTemplate = subBtns[0];	

		selectedTemplate->setSelected(true);			
	}
	else if(selectedTemplate)	
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

	if(selectedTemplate)	
		selectedTemplate->setSelected(false);	
	
	selectedTemplate = subBtns[activeIndex];
	selectedTemplate->setSelected(true);

	if(!isSelected)
		mouseUpSignal(event);	
}

void TemplateButton2::drawLayout()
{	
	gl::pushMatrices();	
	gl::scale(_scale, _scale);	
	gl::draw(templates[1]);
	gl::popMatrices();	

	//gl::draw(photoTemplates[activeIndex][FormatID::FORMAT2_BIG]);
	shader->render(photoTemplates[activeIndex][FormatID::FORMAT2_BIG]);

	gl::pushMatrices();	
	gl::scale(_scale,  _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[0]);	
	gl::popMatrices();		

	gl::draw(lineTexture, Vec2f(49,  getHeight() + 14));
	gl::draw(lineTexture, Vec2f(153, getHeight() + 14));
	gl::draw(lineTexture, Vec2f(246, getHeight() + 14));

	TemplateButton::drawLayout();
}

void TemplateButton2::setSelectRamkaTexture(const ci::gl::Texture& texture)
{	
	for (int i = 0; i < subBtns.size(); i++)	
		subBtns[i]->setSelectRamkaTexture(texture);
}

void TemplateButton2::setLineTexture(const ci::gl::Texture& texture)
{
	lineTexture = texture;
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton3
//
////////////////////////////////////////////////////////////////////////////

TemplateButton3::TemplateButton3(const ci::Rectf& rect, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:TemplateButton(rect, THIRD, templates, stickers)
{
	_scale = 303.0f / templates[3].getWidth();
	_scale1 = ((665.0f-77.0f)/stickers[1].getWidth());
	shiftX = 4.0f;
}

void TemplateButton3::drawLayout()
{	
	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::draw(templates[3]);
	gl::popMatrices();
	
	gl::pushMatrices();	
	//gl::draw(photoTemplates[0][FormatID::FORMAT4_BIG]);	
	shader->render(photoTemplates[0][FormatID::FORMAT4_BIG]);	
	gl::translate(101.5, 0);
	//gl::draw(photoTemplates[1][FormatID::FORMAT4_BIG]);
	shader->render(photoTemplates[1][FormatID::FORMAT4_BIG]);
	gl::translate(101.5, 0);
	//gl::draw(photoTemplates[2][FormatID::FORMAT4_BIG]);	
	shader->render(photoTemplates[2][FormatID::FORMAT4_BIG]);	
	gl::popMatrices();

	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[1]);	
	gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
	gl::draw(stickers[1]);	
	gl::translate(stickers[1].getWidth() + shiftX, 0.0f);
	gl::draw(stickers[1]);	
	gl::popMatrices();	

	TemplateButton::drawLayout();
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton4
//
////////////////////////////////////////////////////////////////////////////

TemplateButton4::TemplateButton4(const ci::Rectf& rect, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:TemplateButton(rect, FOURTH, templates, stickers)
{
	_scale = 202.0f / templates[2].getWidth();
	_scale1 = ((float)templates[2].getWidth() / stickers[3].getWidth());
	_scale2 = 101.0f / (812 - 610);
	shiftY = 1.0f;
}

void TemplateButton4::drawLayout()
{	
	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::draw(templates[2]);
	gl::popMatrices();

	shader->render(photoTemplates[0][FormatID::FORMAT3_BIG]);
	gl::pushMatrices();
	gl::translate(Vec2f(0.0f, 203.5f));
	shader->render(photoTemplates[1][FormatID::FORMAT3_SMALL]);	
	gl::popMatrices();
	//gl::draw(photoTemplates[0][FormatID::FORMAT3_BIG]);	
	//gl::draw(photoTemplates[1][FormatID::FORMAT3_SMALL], Vec2f(0.0f, 203.5f));	

	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[3]);	
	gl::translate(0.0f, stickers[3].getHeight() + shiftY);
	gl::scale(_scale2, _scale2);
	gl::draw(stickers[3]);	
	gl::popMatrices();

	TemplateButton::drawLayout();
}

////////////////////////////////////////////////////////////////////////////
//
//					TemplateButton5
//
////////////////////////////////////////////////////////////////////////////

TemplateButton5::TemplateButton5(const ci::Rectf& rect, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:TemplateButton(rect, FIFTH, templates, stickers)
{
	_scale  = 303.0f / templates[4].getWidth();
	_scale1 = (917.0f / stickers[0].getWidth());	
	_scale2 = (101.0f / stickers[3].getWidth());	
}

void TemplateButton5::drawLayout()
{	
	gl::pushMatrices();	
	gl::scale(_scale, _scale);	
	gl::draw(templates[4]);
	gl::popMatrices();

	shader->render(photoTemplates[0][FormatID::FORMAT2_MIDDLE]);

	gl::pushMatrices();
	gl::translate(Vec2f(158, 0.0f));
	shader->render(photoTemplates[0][FormatID::FORMAT3_SMALL]);	
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate(Vec2f(158, 102.0f));
	shader->render(photoTemplates[1][FormatID::FORMAT3_SMALL]);	
	gl::popMatrices();

	//gl::draw(photoTemplates[0][FormatID::FORMAT3_SMALL], Vec2f(158, 0.0f));	
	//gl::draw(photoTemplates[0][FormatID::FORMAT3_SMALL], Vec2f(158, 102.0f));	

	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[0]);	
	gl::popMatrices();	

	

	gl::pushMatrices();
	gl::translate(156.0f + 2, 0);	
	gl::scale(_scale2, _scale2);
	gl::draw(stickers[3]);
	gl::translate(0, stickers[3].getHeight());	
	gl::draw(stickers[3]);
	gl::popMatrices();	

	TemplateButton::drawLayout();
}

////////////////////////////////////////////////////////////////////////////
//
//					SubButton
//
////////////////////////////////////////////////////////////////////////////

SubButton::SubButton(const ci::Rectf& rect, subID id)
	:SimpleSpriteButton(rect, SubPhotoTemplateChooseEventRef(new SubPhotoTemplateChooseEvent(id))),
	selected(false)
{

}

void SubButton::drawLayout()
{	
	gl::color(Color(1,0,0));//::hex(0xdedddd));
	gl::drawSolidRect(Rectf(0, 0, getWidth(), getHeight()), 0);	
	gl::color(Color::white());

	if (photo)
		gl::draw(photo);

	if(selected)
	{		
		//gl::color(Color::hex(0xe4cf97));	
		//drawtool().drawStrokedRect(Rectf(0, 0, getWidth(), getHeight()), 4);	
		//gl::color(Color::white());	
		gl::draw(ramka);
	}
}

void SubButton::setSelectRamkaTexture(const ci::gl::Texture& texture)
{
	ramka = texture;
}

void SubButton::setSelected(bool value)
{
	selected = value;
}

void SubButton::setPhoto(const ci::gl::Texture& tex, BaseShaderRef shader)
{
	gl::Fbo fbo = gl::Fbo(tex.getWidth(), tex.getHeight());

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		shader->render(tex);
	});

	photo = fbo.getTexture();
	Utils::clearFBO(fbo);	
}