#pragma once

#include "settings/StatisticEvent.h"
#include "settings/GameConfEvent.h"
#include "ToolRoundBtn.h"

namespace kubik
{
	namespace config
	{
		class StatToolButton : public ToolRoundBtn
		{
		public:			
			StatToolButton(Rectf rect, GameId gameId, TextItem texti)
				:ToolRoundBtn(rect, texti.getText(), texti.getFont(), Color::hex(0xffff00))
			{
				event = StatisticEventRef(new StatisticEvent(gameId));
			}	
		};

		class ConfToolButton : public ToolRoundBtn
		{
		public:
			ConfToolButton(Rectf rect, GameId gameId, TextItem texti)
				:ToolRoundBtn(rect, texti.getText(), texti.getFont(), Color::hex(0xffff00))
			{
				event = GameConfEventRef(new GameConfEvent(gameId));
			}
		};

		typedef std::shared_ptr<StatToolButton> StatToolButtonRef;
		typedef std::shared_ptr<ConfToolButton> ConfToolButtonRef;
	}
}