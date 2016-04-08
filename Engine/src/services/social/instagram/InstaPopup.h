#pragma once
#include "cinder/app/AppNative.h"
#include "gui/Sprite.h"
#include "cinder/Timeline.h"
#include "instagram/InstagramClient.h"
#include "gui/ImageButtonSprite.h"

namespace instagram
{
	typedef std::shared_ptr<class InstaPopup> InstaPopupRef;

	class InstaPopup : public kubik::Sprite
	{
		typedef ci::signals::signal<void(void)> SignalVoid;

	public:
		static const int CLOSE_POPUP = 0;
		static const int PRINT = 1;

		explicit InstaPopup(InstagramClientRef client, const ci::gl::Texture& close, const ci::gl::Texture& save, const ci::gl::Texture& _template);

		void draw() override;
		void setAlpha(float alpha) override;

		void show(const ImageGraphic& image, const ci::EaseFn& = ci::EaseOutCubic(), float time = 0.7f);
		void hide(const ci::EaseFn& = ci::EaseOutCubic(), float time = 0.7f);
		void showAnimationFinish();
		void hideAnimationFinish();

		void activateListeners();
		void unActivateListeners();

		SignalVoid touchedEvent;

		bool isOpen();
		void setDesignElements(const ci::gl::Texture& close, const ci::gl::Texture& print, const ci::gl::Texture& _template);

	private:
		ci::Vec2f imageShift;
		ci::gl::Texture closeImage, saveImage, templateImage;
		ci::ColorA bgColor;
		ci::Anim<float> alpha;
		ci::Anim<ci::Vec2f> imagePositionAnim;		
		InstagramClientRef client;
		kubik::ImageButtonSpriteRef closeBtn, printBtn;
		ImageGraphic image;
		bool showing;

		void hiding(kubik::EventGUIRef& event);
		void printing(kubik::EventGUIRef& event);
	};
}
