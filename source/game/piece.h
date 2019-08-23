#pragma once

#include "defs.h"
#include "position.h"
#include "gamecommon.h"
#include "ipiecemovement.h"
#include <vector>
#include <memory>

namespace Chess
{

struct SerializedPiece
{
	Type type_ = Type::Pawn;
	Color color_ = Color::White;
	bool hasMoved_ = false;
};

class Piece
{
public:

	Piece(Type type, Color color);
	Piece(const SerializedPiece&);
	virtual ~Piece() {}

	Color GetColor() const;
	Type GetType() const;
	Pos GetPos() const;
	Game* GetGame() const;
	void SetPos(const Pos& position);
	void SetGame(Game* game);
	IPieceMovement& GetMovement();
	bool HasMoved() const;
	void SetHasMoved(bool value);
	SerializedPiece Serialize() const;

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
