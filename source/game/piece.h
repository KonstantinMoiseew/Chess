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
    PieceType type_ = PieceType::Pawn;
	Color color_ = Color::White;
	bool hasMoved_ = false;

};

class Piece
{
public:

    Piece(PieceType type, Color color);
	Piece(const SerializedPiece&);
	virtual ~Piece() {}

	Color GetColor() const;
    PieceType GetType() const;
    Pos GetPos() const;
	Game* GetGame() const;
	void SetPos(const Pos& position);
    void SetType(const PieceType &type) { type_=type; }
	void SetGame(Game* game);
	IPieceMovement& GetMovement();
	bool HasMoved() const;
	void SetHasMoved(bool value);
	SerializedPiece Serialize() const;

protected:

	Color color_ = Color::White;
    PieceType type_ = PieceType::Pawn;
	Pos position_;
	Game* game_ = nullptr;
	std::unique_ptr<IPieceMovement> movement_;
	bool hasMoved_ = false;
};

using PieceUnPtr = std::unique_ptr<Piece>;
using Pieces = std::vector<PieceUnPtr>;

}
