#pragma once

#include "gamecommon.h"
#include "commands.h"

namespace Chess
{

class History
{
public:

    bool Execute(Game& game, ICommand* command);
	void RollbackLast(Game& game);
	void ClearHistory();

	int GetSize() const;
	ICommand* GetCommand(int index);
	bool king_ = false;
	Piece *piece_changed_;

protected:

	std::vector<ICommandUnPtr> history_;

};

}
