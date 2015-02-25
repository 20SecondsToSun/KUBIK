#pragma once

#include "TextTools.h"
#include "OpenSystemDirectoryEvent.h"
#include "gui/SimpleSpriteButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class LoadButton> LoadButtonRef;

		class LoadButton: public SimpleSpriteButton
		{
		public:
			LoadButton(const std::string &path, const ci::Rectf& rect, const TextItem& textItem, const ci::gl::Texture& icon);

			virtual void drawLayout();
			virtual void draw();
			void draw(float x, float y);			
			void setAlpha(float alpha);

		private:
			TextItem textItem;			
			ci::ColorA bckColor;
			ci::gl::Texture tex, icon;
			float texPosX;			
		};	
	}
}