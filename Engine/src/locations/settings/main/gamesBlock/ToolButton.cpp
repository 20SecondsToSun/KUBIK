#include "main/gamesBlock/ToolButton.h"

using namespace kubik;
using namespace kubik::config;

StatToolButton::StatToolButton(const Rectf& rect, const GameId& gameId, const TextItem& text)
	:ToolRoundBtn(rect, text.getText(), text.getFont(), Color::hex(0xffff00))
{
	event = StatisticEventRef(new StatisticEvent(gameId));
}

ConfToolButton::ConfToolButton(const Rectf& rect, const GameId& gameId, const TextItem& text)
	:ToolRoundBtn(rect, text.getText(), text.getFont(), Color::hex(0xffff00))
{
	event = GameConfEventRef(new GameConfEvent(gameId));
}