#include "history.h"
#include "game.h"

bool Chess::History::Execute(Game& game, ICommand* command)
{
	if (command->Validate(game))
	{
		history.emplace_back(command);
		command->Do(game);
		game.NextPlayerTurn();
		return true;
	}

	return false;
}

void Chess::History::RollbackLast(Game& game)
{
	if (!history.empty())
	{
		history.back()->Undo(game);
		history.pop_back();
		game.NextPlayerTurn();
	}
}
