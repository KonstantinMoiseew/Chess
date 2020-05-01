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
	virtual Type GetPieceType() const = 0;
	virtual Color GetPieceColor() const = 0;
	virtual bool DidCapture() const = 0;
	virtual Type GetCapturedPieceType() const = 0;
	virtual Color GetCapturedPieceColor() const = 0;
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
	Type GetPieceType() const override;
	Color GetPieceColor() const override;
	bool DidCapture() const override;
	Type GetCapturedPieceType() const override;
	Color  GetCapturedPieceColor() const override;

protected:

	Pos posFrom_;
	Pos posTo_;
	Type pieceType_ = Type::Pawn;
	Color pieceColor_=Color::White;
	bool pieceHasMovedBefore_ = false;
	std::optional<SerializedPiece> enemyPiece_;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
