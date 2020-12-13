#include "piece.h"
#include "defs.h"
#include "gamecommon.h"
#include "game.h"
#include "iobserver.h"
#include "piecemovements.h"

Chess::Piece::Piece(PieceType type, Color color)
{
	color_ = color;
	type_ = type;

	switch(type)
	{
    case PieceType::Rook: movement_.reset(new RookMovement(*this)); break;
    case PieceType::Bishop: movement_.reset(new BishopMovement(*this)); break;
    case PieceType::Knight: movement_.reset(new KnightMovement(*this)); break;
    case PieceType::Queen: movement_.reset(new QueenMovement(*this)); break;
    case PieceType::King: movement_.reset(new KingMovement(*this)); break;
    case PieceType::Pawn: movement_.reset(new PawnMovement(*this)); break;
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

Chess::PieceType Chess::Piece:: GetType() const
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
