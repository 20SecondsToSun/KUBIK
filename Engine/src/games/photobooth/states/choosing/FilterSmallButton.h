#pragma once
#include "gui/SimpleSpriteButton.h"
#include "FilterChangedEvent.h"
#include "TextTools.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class FilterSmallButton> FilterSmallButtonRef;

			class FilterSmallButton: public SimpleSpriteButton
			{	
				bool isSelected;
				int id;
				std::string text;
				ci::Font fontC, fontO;

				ci::gl::Texture titleSmall, titleBig, photo, background;
				ci::Vec2f titleSmallPos, titleBigPos;

				shaders::imagefilters::BaseShaderRef shader;

				enum stateID
				{
					OPEN,
					CLOSE
				}
				state;

				float getScaleBetweenStates();

			public:
				FilterSmallButton(const ci::Vec2f& vec,
					int id, 
					const std::string &text,
					const ci::gl::Texture &background, 
					ci::Font fontC, 
					ci::Font fontO);

				virtual void drawLayout();

				void setSelected(bool value);				
				int getID()		const;
				bool selected() const;

				void setOpenState();
				void setCloseState();

				void setPhoto(const ci::gl::Texture& tex);
			};
		}
	}
}