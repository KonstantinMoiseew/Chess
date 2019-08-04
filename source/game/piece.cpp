#include "piece.h"
#include "defs.h"
#include "gamecommon.h"
#include "game.h"
#include "iobserver.h"
#include "piecemovements.h"

Chess::Piece::Piece(Type type, Color color, Pos position)
{
	color_ = color;
	type_ = type;
	position_ = position;

	switch(type)
	{
	case Type::Rook: movement_.reset(new RookMovement(*this)); break;
	case Type::Bishop: movement_.reset(new BishopMovement(*this)); break;
	case Type::Knight: movement_.reset(new KnightMovement(*this)); break;
	case Type::Queen: movement_.reset(new QueenMovement(*this)); break;
	case Type::King: movement_.reset(new KingMovement(*this)); break;
	case Type::Pawn: movement_.reset(new PawnMovement(*this)); break;
	default: break;
	}
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

Chess::IPieceMovement& Chess::Piece::GetMovement()
{
	return *movement_;
}

void Chess::Piece:: SetPos(const Pos& pos)
{
	if(!(position_==pos))
	{
		MoveFirst();
	}
	position_ = pos;
	OBS_CALL(game_->GetObservers(), OnPieceMoved(*this));
}

bool Chess::Piece:: GetMoveFirst()
{
	return move_first_;
}

void Chess::Piece::MoveFirst()
{
	move_first_=true;
}

















