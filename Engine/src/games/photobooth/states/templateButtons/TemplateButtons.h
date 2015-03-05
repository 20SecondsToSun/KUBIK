#pragma once
#include "gui/SimpleSpriteButton.h"
#include "PhotoTemplateChooseEvent.h"
#include "SubPhotoTemplateChooseEvent.h"
#include "DrawTools.h"

namespace kubik
{
	typedef std::shared_ptr<class TemplateButton>  TemplateButtonRef;
	typedef std::shared_ptr<class TemplateButton1> TemplateButton1Ref;
	typedef std::shared_ptr<class TemplateButton2> TemplateButton2Ref;
	typedef std::shared_ptr<class TemplateButton3> TemplateButton3Ref;
	typedef std::shared_ptr<class TemplateButton4> TemplateButton4Ref;
	typedef std::shared_ptr<class TemplateButton5> TemplateButton5Ref;

	typedef std::shared_ptr<class SubButton> SubButtonRef;

	enum templateID
	{
		FIRST,
		SECOND,
		THIRD,
		FOURTH,
		FIFTH
	};

	enum subID
	{
		SUB_FIRST,
		SUB_SECOND,
		SUB_THIRD
	};

	class TemplateButton: public SimpleSpriteButton
	{
	protected:
		float _scale, _scale1;
		std::vector<ci::gl::Texture> templates, stickers;
		bool isSelected;
		templateID id;

	public:
		TemplateButton(const ci::Rectf& rect, templateID id, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void setSelected(bool value);
		virtual void drawLayout();
		templateID getID();
		virtual void init(){};
	};

	class TemplateButton1: public TemplateButton
	{
		float shiftY;

	public:
		TemplateButton1(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void drawLayout();
	};

	class TemplateButton2: public TemplateButton
	{		
		std::vector<SubButtonRef> subBtns;
		void photoTemplateChoose(EventGUIRef& event);
		SubButtonRef selectedTemplate;

	public:
		TemplateButton2(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void drawLayout();
		virtual void activateListeners();
		virtual void unActivateListeners();
		virtual void setSelected(bool value);
		virtual void init();
	};

	class TemplateButton3: public TemplateButton
	{
		float shiftY, shiftX;

	public:
		TemplateButton3(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void drawLayout();
	};

	class TemplateButton4: public TemplateButton
	{
		float _scale2, shiftY;

	public:
		TemplateButton4(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void drawLayout();
	};

	class TemplateButton5: public TemplateButton
	{
		float _scale2;

	public:
		TemplateButton5(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
		virtual void drawLayout();
	};

	class SubButton: public SimpleSpriteButton
	{
		bool selected;
	public:
		SubButton(const ci::Rectf& rect, subID id);	
		virtual void drawLayout();
		void setSelected(bool value);
	};
}	