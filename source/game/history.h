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

protected:

	std::vector<ICommandUnPtr> history_;
};

}
