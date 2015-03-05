#include "states/PhotoTemplate.h"

using namespace kubik;
using namespace ci;
using namespace std;

TemplateButton::TemplateButton(const Rectf& rect, templateID id, const vector<gl::Texture>& templates, const vector<gl::Texture>& stickers)
	:SimpleSpriteButton(rect, PhotoTemplateChooseEventRef(new PhotoTemplateChooseEvent(id))),
	templates(templates),
	stickers(stickers),
	isSelected(false),
	id(id)
{

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
		gl::color(Color::hex(0xe4cf97));		
		drawtool().drawStrokedRect(Rectf(0, 0, getWidth(), getHeight()), 4);
		gl::color(Color::white());
	}
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

	subBtns.push_back(SubButtonRef(new SubButton(Rectf(16.0f, 509.0f, 83.0f, 605.0f),   SUB_FIRST)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(118.0f, 509.0f, 185.0f, 605.0f), SUB_SECOND)));
	subBtns.push_back(SubButtonRef(new SubButton(Rectf(220.0f, 509.0f, 287.0f, 605.0f), SUB_THIRD)));

	addChild(subBtns[0]);
	addChild(subBtns[1]);
	addChild(subBtns[2]);
}

void TemplateButton2::init()
{
	selectedTemplate = nullptr;
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
	auto id = eventref->getTemplateID();

	if(selectedTemplate)	
		selectedTemplate->setSelected(false);	
	
	selectedTemplate = subBtns[id];
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

	gl::pushMatrices();	
	gl::scale(_scale,  _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[0]);	
	gl::popMatrices();	

	gl::color(Color::hex(0xe4cf97));
	drawtool().drawDashedLine(49,  getHeight() + 14,  49, getHeight() + 14 + 32, 2, 2 );
	drawtool().drawDashedLine(153, getHeight() + 14, 153, getHeight() + 14 + 32, 2, 2 );
	drawtool().drawDashedLine(246, getHeight() + 14, 246, getHeight() + 14 + 32, 2, 2 );
	gl::color(Color::white());

	TemplateButton::drawLayout();
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
	_scale2 = (101.0f/ stickers[3].getWidth());	
}

void TemplateButton5::drawLayout()
{	
	gl::pushMatrices();	
	gl::scale(_scale, _scale);	
	gl::draw(templates[4]);
	gl::popMatrices();

	gl::pushMatrices();
	gl::scale(_scale, _scale);	
	gl::scale(_scale1, _scale1);	
	gl::draw(stickers[0]);	
	gl::popMatrices();	

	gl::pushMatrices();
	gl::translate(156.0f + 1, 0);	
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

	if(selected)
	{		
		gl::color(Color::hex(0xe4cf97));	
		drawtool().drawStrokedRect(Rectf(0, 0, getWidth(), getHeight()), 4);	
		gl::color(Color::white());	
	}
}

void SubButton::setSelected(bool value)
{
	selected = value;
}