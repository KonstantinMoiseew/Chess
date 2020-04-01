#pragma once
#include "gamecommon.h"
#include "position.h"
#include "piece.h"
#include <optional>
#include <QPixmap>

namespace Chess
{

class ICommand
{
public:

	virtual ~ICommand() {}

	virtual bool Validate(const Game& game) const = 0;
	virtual void Do(Game& game) = 0;
	virtual void Undo(Game& game) = 0;

	virtual std::string ToString() const = 0;
	virtual QPixmap ToPix() const = 0;
};


class MoveCommand : public ICommand
{
public:

	MoveCommand(Piece& piece, const Pos& pos);
	MoveCommand(const Pos& pos_from, const Pos& pos_to);

	bool Validate(const Game& game) const override;
	void Do(Game& game) override;
	void Undo(Game& game) override;

	std::string ToString() const override;
	 QPixmap ToPix() const override;
protected:

	Pos posFrom_;
	Pos posTo_;
	Type pieceType_ = Type::Pawn;
	Color pieceColore_=Color::White;
	bool pieceHasMovedBefore_ = false;
	std::optional<SerializedPiece> enemyPiece_;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
