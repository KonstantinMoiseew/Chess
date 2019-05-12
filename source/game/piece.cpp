#include "piece.h"
#include "defs.h"
#include "gamecommon.h"

Chess::Piece::Piece(Type type, Color color, Pos position)
{
	color_ = color;
	type_ = type;
	position_ = position;
}

Chess:: Color Chess::Piece:: GetColor()
{
	return color_;
}

Chess::Type Chess::Piece:: GetType()
{
	return type_;
}
Chess::Pos Chess::Piece:: GetPos()
{
	return position_;
}
