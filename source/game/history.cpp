#include "history.h"
#include "game.h"

bool Chess::History::Execute(Game& game, ICommand* command, bool flag_changable)
{
	if (!command->Validate(game,flag_changable))
		return false;

	command->Do(game,flag_changable);

	if (!game.IsKingAttacked(game.GetPlayerTurn()))
	{
		if (game.HasKingAttackedAfterMove(game.GetPlayerTurn()))
		{
			king_=true;
		}
		else
			king_=false;

		command->SetKingUnderAttak(king_);
		history_.emplace_back(command);
		if (!flag_changable)
			game.NextPlayerTurn();

		OBS_CALL(game.GetObservers(), OnCommandExecuted(*command));

		return true;
	}
	else
	{
		if(!flag_changable)
		{
			command->Undo(game)	;
			delete command;
			return false;
		}
		history_.emplace_back(command);
		return true;
	}
}




void Chess::History::RollbackLast(Game& game)
{
	if (!history_.empty())
	{

		history_.back()->Undo(game,history_.back()->GetFlagChangable());
		history_.pop_back();
		game.NextPlayerTurn();
	}
}

void Chess::History:: ClearHistory()
{
	history_.clear();
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















