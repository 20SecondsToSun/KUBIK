#pragma once
#include "gui/SimpleSpriteButton.h"
#include "PhotoTemplateChooseEvent.h"
#include "SubPhotoTemplateChooseEvent.h"
#include "DrawTools.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
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
				ci::gl::Texture btnOver, btnOverText;
				vector<std::map<FormatID, ci::gl::Texture>> photoTemplates;

				static shaders::imagefilters::BaseShaderRef shader;

				virtual void drawTemplateBg() = 0;
				virtual void drawPhotos() = 0;
				virtual void drawStickers() = 0;

			public:
				TemplateButton(const ci::Rectf& rect, templateID id, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
				virtual void setSelected(bool value);
				virtual void drawLayout();		
				virtual void init(){};
				void setSelectDesign(const ci::gl::Texture& btn);
				templateID getID();
				void setPhotoTemplates(const vector<std::map<FormatID, ci::gl::Texture>>& photoTemplates, shaders::imagefilters::BaseShaderRef shader);
				virtual ci::gl::Texture getPrintTemplate(){ return ci::gl::Texture(); };
			};

			class TemplateButton1: public TemplateButton
			{
				float shiftY;
				virtual void drawTemplateBg();
				virtual void drawPhotos();
				virtual void drawStickers();
			public:
				TemplateButton1(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);	
				virtual ci::gl::Texture getPrintTemplate();
			};

			class TemplateButton2: public TemplateButton
			{		
				std::vector<SubButtonRef> subBtns;
				void photoTemplateChoose(EventGUIRef& event);
				SubButtonRef selectedTemplate;
				int activeIndex;
				ci::gl::Texture selectRamkaTexture, lineTexture;

				virtual void drawTemplateBg();
				virtual void drawPhotos();
				virtual void drawStickers();
			public:
				TemplateButton2(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
				virtual void activateListeners();
				virtual void unActivateListeners();
				virtual void setSelected(bool value);
				virtual void init();
				void setSelectRamkaTexture(const ci::gl::Texture& texture);
				void setLineTexture(const ci::gl::Texture& texture);
				virtual ci::gl::Texture getPrintTemplate();				
			};

			class TemplateButton3: public TemplateButton
			{
				float shiftY, shiftX;
				virtual void drawTemplateBg();
				virtual void drawPhotos();
				virtual void drawStickers();
			public:
				TemplateButton3(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
				ci::gl::Texture getPrintTemplate();
			};

			class TemplateButton4: public TemplateButton
			{
				float _scale2, shiftY;
				virtual void drawTemplateBg();
				virtual void drawPhotos();
				virtual void drawStickers();
			public:
				TemplateButton4(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
				ci::gl::Texture getPrintTemplate();
			};

			class TemplateButton5: public TemplateButton
			{
				float _scale2;
				virtual void drawTemplateBg();
				virtual void drawPhotos();
				virtual void drawStickers();
			public:
				TemplateButton5(const ci::Rectf& rect, const std::vector<ci::gl::Texture>& templates, const std::vector<ci::gl::Texture>& stickers);
				ci::gl::Texture getPrintTemplate();
			};

			class SubButton: public SimpleSpriteButton
			{
				bool selected;
				ci::gl::Texture photo, ramka;

			public:
				SubButton(const ci::Rectf& rect, subID id);	
				virtual void drawLayout();
				void setSelected(bool value);
				void setPhoto(const ci::gl::Texture& tex, shaders::imagefilters::BaseShaderRef shader);
				void setSelectRamkaTexture(const ci::gl::Texture& texture);
			};
		}
	}
}	