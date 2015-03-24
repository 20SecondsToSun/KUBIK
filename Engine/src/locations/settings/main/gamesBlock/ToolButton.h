#pragma once

#include "settings/StatisticEvent.h"
#include "settings/GameConfEvent.h"
#include "ToolRoundBtn.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class StatToolButton> StatToolButtonRef;
		typedef std::shared_ptr<class ConfToolButton> ConfToolButtonRef;

		class StatToolButton : public ToolRoundBtn
		{
		public:
			StatToolButton(const Rectf& rect, const GameId& gameId, const TextItem& text);
		};

		class ConfToolButton : public ToolRoundBtn
		{
		public:
			ConfToolButton(const Rectf& rect, const GameId& gameId, const TextItem& text);
		};		
	}
}