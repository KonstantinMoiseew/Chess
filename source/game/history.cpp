#include "history.h"
#include "game.h"

bool Chess::History::Execute(Game& game, ICommand* command)
{
	if (command->Validate(game))
	{
		history_.emplace_back(command);
		command->Do(game);
		game.NextPlayerTurn();
		return true;
	}

	return false;
}

void Chess::History::RollbackLast(Game& game)
{
	if (!history_.empty())
	{
		history_.back()->Undo(game);
		history_.pop_back();
		game.NextPlayerTurn();
	}
}

int Chess::History::GetSize() const
{
	return static_cast<int>(history_.size());
}

Chess::ICommand* Chess::History::GetCommand(int index)
{
	if (index >= 0 && index < GetSize())
		return history_[static_cast<unsigned int>(index)].get();
	else
		return nullptr;
}
