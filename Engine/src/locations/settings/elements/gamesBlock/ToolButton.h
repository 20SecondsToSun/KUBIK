#pragma once

#include "IDrawable.h"
#include "Button.h"
#include "settings/StatisticEvent.h"
#include "settings/GameConfEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolButton> ToolButtonRef;

		class ToolButton: public Button
		{
		public:
			ToolButton(Rectf rectf):Button(rectf)
			{
			}

			ToolButton():Button(Rectf(0,0,0,0))
			{
			}

			virtual void draw()
			{
				textTools().textFieldDraw(text, &font, color, Vec2f(buttonArea.x1 + 25, buttonArea.y1 + 8));
				gl::color(color);
				gl::drawStrokedRoundedRect(buttonArea, 8, 200);
				gl::color(Color::white());
			}

			void setFont(Font font)
			{
				this->font = font;
			}

			void setText(string text)
			{
				this->text = text;
			}

			void setColor(Color color)
			{
				this->color = color;
			}

		private:
			string text;
			Font font;
			Color color;
		};		

		class StatToolButton : public ToolButton
		{
		public:
			StatToolButton(GameId gameId, Rectf rectf):ToolButton(rectf)
			{
				event = StatisticEventRef(new StatisticEvent(gameId));
			}

			StatToolButton(GameId gameId):ToolButton(Rectf(0,0,0,0))
			{
				event = StatisticEventRef(new StatisticEvent(gameId));
			}			
		};

		class ConfToolButton : public ToolButton
		{
		public:
			ConfToolButton(GameId gameId,Rectf rectf):ToolButton(rectf)
			{
				event = GameConfEventRef(new GameConfEvent(gameId));
			}

			ConfToolButton(GameId gameId):ToolButton(Rectf(0,0,0,0))
			{
				event = GameConfEventRef(new GameConfEvent(gameId));
			}
		};

		typedef std::shared_ptr<StatToolButton> StatToolButtonRef;
		typedef std::shared_ptr<ConfToolButton> ConfToolButtonRef;
	}
}