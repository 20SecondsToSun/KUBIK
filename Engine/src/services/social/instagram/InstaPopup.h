#pragma once
#include "cinder/app/AppNative.h"
#include "gui/Sprite.h"
#include "cinder/Timeline.h"
#include "instagram/InstagramClient.h"
#include "gui/ImageButtonSprite.h"

using namespace kubik;

namespace instagram
{
	typedef std::shared_ptr<class InstaPopup> InstaPopupRef;

	class InstaPopup : public Sprite
	{
		typedef ci::signals::signal<void(void)> SignalVoid;

	public:
		static const int CLOSE_POPUP = 0;
		static const int PRINT = 1;

		InstaPopup(InstagramClientRef client, const gl::Texture& close, const gl::Texture& save, const gl::Texture& _template);

		virtual void draw();
		virtual void setAlpha(float alpha);

		void show(const ImageGraphic& image, const ci::EaseFn& = ci::EaseOutCubic(), float time = 0.7f);
		void hide(const ci::EaseFn& = ci::EaseOutCubic(), float time = 0.7f);
		void alphAnimationUpdate();
		void showAnimationFinish();
		void hideAnimationFinish();

		void activateListeners();
		void unActivateListeners();

		SignalVoid touchedEvent;

		bool isOpen();

	private:
		ci::gl::Texture closeImage, saveImage, templateImage;
		ci::ColorA bgColor;
		ci::Anim<float> alpha;
		InstagramClientRef client;
		ImageButtonSpriteRef closeBtn, printBtn;
		bool showing;
		ImageGraphic image;

		void hiding(EventGUIRef& event);
		void printing(EventGUIRef& event);
	};
}
