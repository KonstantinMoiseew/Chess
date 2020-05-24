#include "piece.h"
#include "defs.h"
#include "gamecommon.h"
#include "game.h"
#include "iobserver.h"
#include "piecemovements.h"

Chess::Piece::Piece(Type type, Color color)
{
	color_ = color;
	type_ = type;

	switch(type)
	{
	case Type::Rook: movement_.reset(new RookMovement(*this)); break;
	case Type::Bishop: movement_.reset(new BishopMovement(*this)); break;
	case Type::Knight: movement_.reset(new KnightMovement(*this)); break;
	case Type::Queen: movement_.reset(new QueenMovement(*this)); break;
	case Type::King: movement_.reset(new KingMovement(*this)); break;
	case Type::Pawn: movement_.reset(new PawnMovement(*this)); break;
	}
}

Chess::Piece::Piece(const SerializedPiece& ser_piece)
	: Piece(ser_piece.type_, ser_piece.color_)
{
	hasMoved_ = ser_piece.hasMoved_;
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
	position_ = pos;
	if (game_)
	{
		OBS_CALL(game_->GetObservers(), OnPieceMoved(*this));
	}

	if((position_.y_==0||position_.y_==7)&&this->type_==Chess::Type::Pawn)
	{
		OBS_CALL(game_->GetObservers(), OnShowChose(*this));
	}
}

void Chess::Piece::SetHasMoved(bool value)
{
	hasMoved_ = value;
}

Chess::SerializedPiece Chess::Piece::Serialize() const
{
	return Chess::SerializedPiece{GetType(), GetColor(), HasMoved()};
}

bool Chess::Piece:: HasMoved() const
{
	return hasMoved_;
}
