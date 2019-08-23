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

protected:

	std::vector<ICommandUnPtr> history;
};

}
