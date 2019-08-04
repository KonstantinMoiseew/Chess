#pragma once

#include "defs.h"
#include "position.h"
#include "gamecommon.h"
#include "ipiecemovement.h"
#include <vector>
#include <memory>

namespace Chess
{

class Piece
{
public:

	Piece(Type type, Color color, Pos position);
	virtual ~Piece() {}

	Color GetColor() const;
	Type GetType() const;
	Pos GetPos() const;
	Game* GetGame() const;
	void SetPos(const Pos& position);
	void SetGame(Game* game);
	IPieceMovement& GetMovement();
	void ResetMovementFlag();
	bool HasMoved() const;

protected:

	Color color_ = Color::White;
	Type type_ = Type::Pawn;
	Pos position_;
	Game* game_ = nullptr;
	std::unique_ptr<IPieceMovement> movement_;
	bool hasMoved_ = false;
};

using PieceUnPtr = std::unique_ptr<Piece>;
using Pieces = std::vector<PieceUnPtr>;

}
