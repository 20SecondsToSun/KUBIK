#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "Utils.h"

namespace kubik
{
	namespace config
	{
		class Title:public Sprite
		{
		public:	
			Title(ConfigSettingsRef configSettings, Vec2i position)				
				 :Sprite(),
				 name(configSettings->getActionName()),
				 font(configSettings->getFont("introLight44")),
				 color(ci::Color::white())
			{
				setPosition(position);
			}

			virtual void drawLayout()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(name, &font, color, ci::Vec2f(-12, 3));				
			}

			void setAlpha(float alpha)
			{
				color = Utils::colorAlpha(color, alpha);	
			}

			void setActivityName(const std::string& name)
			{
				this->name = name;
			}

		private:
			std::string name;				
			ci::ColorA  color;	
			ci::Font font;
		};

		typedef std::shared_ptr<Title> TitleRef;
	}
}