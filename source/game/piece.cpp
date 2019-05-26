#include "piece.h"
#include "defs.h"
#include "gamecommon.h"
#include "game.h"
#include "iobserver.h"

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

void Chess::Piece:: SetPos(const Pos& pos)
{
	position_ = pos;
	OBS_CALL(game_->GetObservers(), OnPieceMoved(*this));
}
