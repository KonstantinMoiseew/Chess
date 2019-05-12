#include "piece.h"
#include "defs.h"
#include "gamecommon.h"

Chess::Piece::Piece(Type type, Color color, Pos position)
{
	color_ = color;
	type_ = type;
	position_ = position;
}

Chess:: Color Chess::Piece:: GetColor() const
{
	return color_;
}

Chess::Type Chess::Piece:: GetType() const
{
	return type_;
}

Chess::Pos Chess::Piece:: GetPos() const
{
	return position_;
}

Chess::Game* Chess::Piece::GetGame() const
{
	return game_;
}

void Chess::Piece::SetGame(Game* game)
{
	game_ = game;
}
