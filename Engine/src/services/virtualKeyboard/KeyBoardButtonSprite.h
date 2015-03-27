#pragma once
#include "gui/SimpleSpriteButton.h"

namespace kubik
{
	typedef std::shared_ptr<class KeyBoardButtonSprite> KeyBoardButtonSpriteRef;

	class KeyBoardButtonSprite : public SimpleSpriteButton
	{

	public:
		KeyBoardButtonSprite(const ci::gl::Texture& image, ci::Font font, const std::string& label);
		KeyBoardButtonSprite(ci::gl::Texture image, std::string label);

		virtual void drawLayout() override;

		void changeTexture(const Texture& image);		
		void setBtnId(const std::string& value);
		void createTextField();
		void down();
		void setAlpha(float alpha);

		std::string getBtnId() const;

	private:
		ci::gl::Texture image;
		ci::Anim<ci::Color> overColor;
		gl::Texture textTexture;
		std::string code;
		ci::Font font;
		bool isTextField;
	};
}