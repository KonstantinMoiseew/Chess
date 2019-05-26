#pragma once

#include "defs.h"
#include "position.h"
#include <vector>
#include <memory>
#include "gamecommon.h"
#include <QString>

namespace Chess
{

class Piece
{
public:

	Piece(Type type, Color color, Pos position);

	Color GetColor() const;
	Type GetType() const;
	Pos GetPos() const;
	Game* GetGame() const;
	void SetPos(const Pos& position);
	void SetGame(Game* game);

	virtual ~Piece() {}

protected:

	Color color_ = Color::White;
	Type type_ = Type::Pawn;
	Pos position_;
	Game* game_ = nullptr;
};

using PieceUnPtr = std::unique_ptr<Piece>;
using Pieces = std::vector<PieceUnPtr>;

}
