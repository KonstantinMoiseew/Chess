#pragma once
#include "gamecommon.h"
#include "position.h"
#include "piece.h"
#include <optional>

namespace Chess
{

class ICommand
{
public:

	virtual ~ICommand() {}

	virtual bool Validate(Game& game) const = 0;
	virtual void Do(Game& game) = 0;
	virtual void Undo(Game& game) = 0;
};


class MoveCommand : public ICommand
{
public:

	MoveCommand(Piece& piece, const Pos& pos);
	MoveCommand(const Pos& pos_from, const Pos& pos_to);

	bool Validate(Game& game) const override;
	void Do(Game& game) override;
	void Undo(Game& game) override;

protected:

	Pos posFrom_;
	Pos posTo_;
	bool pieceHasMovedBefore_ = false;
	std::optional<SerializedPiece> enemyPiece_;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
